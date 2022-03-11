#include <gflags/gflags.h>
#include <glog/logging.h>

#include <iostream>

#include "core/gameboy.h"
#include "driver/minifb/minifb_display.h"
#include "driver/minifb/minifb_window.h"
#include "driver/stub/stub_display.h"

DEFINE_string(filename, "", "Rom filename");
DEFINE_int32(breakpoint, -1, "PC at breakpoint");
DEFINE_bool(stub, false, "Use stub display");
DEFINE_int32(n_frame, -1, "Number of frames to update");

void runStub(gbeml::GameBoy *gb) {
  int n = FLAGS_n_frame;
  while (n > 0) {
    for (int i = 0; i < 70224; ++i) {
      gb->tick();
    }
    n--;
  }
}

void runMiniFb(gbeml::GameBoy *gb) {
  gbeml::MiniFbWindow window;

  if (!window.init()) {
    std::cerr << "Failed to initialize window." << std::endl;
    return;
  }
  std::cout << "Window init OK" << std::endl;

  do {
    for (int i = 0; i < 70224; ++i) {
      gb->tick();
    }

    gbeml::u32 *buffer = gb->getDisplay()->getBuffer();
    if (!window.update(buffer)) {
      std::cerr << "Failed to update window." << std::endl;
      return;
    }
  } while (window.waitSync());
}

int main(int argc, char *argv[]) {
  google::InitGoogleLogging(argv[0]);
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  if (FLAGS_filename.empty()) {
    std::cout << "Rom filename is empty." << std::endl;
    return 1;
  }

  int breakpoint;
  std::stringstream ss;
  ss << std::hex << FLAGS_breakpoint;
  ss >> breakpoint;

  gbeml::Display *display;
  if (FLAGS_stub) {
    display = new gbeml::StubDisplay();
  } else {
    display = new gbeml::MiniFbDisplay();
  }

  gbeml::GameBoy gb(display, breakpoint);
  if (!gb.init(FLAGS_filename)) {
    std::cout << "Failed to initialize gb." << std::endl;
    return 1;
  }
  std::cout << "GB init OK" << std::endl;

  if (FLAGS_stub) {
    runStub(&gb);
  } else {
    runMiniFb(&gb);
  }

  std::cout << "OK" << std::endl;
  return 0;
}
