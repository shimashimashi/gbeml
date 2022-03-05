#include <gflags/gflags.h>
#include <glog/logging.h>

#include <iostream>

#include "core/gameboy.h"
#include "driver/minifb/minifb_display.h"
#include "driver/minifb/minifb_window.h"

DEFINE_string(filename, "", "Rom filename");

int main(int argc, char *argv[]) {
  google::InitGoogleLogging(argv[0]);
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  if (FLAGS_filename.empty()) {
    std::cout << "Rom filename is empty." << std::endl;
    return 1;
  }

  gbeml::MiniFbDisplay display;
  gbeml::GameBoy gb(&display);
  if (!gb.init(FLAGS_filename)) {
    std::cout << "Failed to initialize gb." << std::endl;
    return 1;
  }
  std::cout << "GB init OK" << std::endl;

  gbeml::MiniFbWindow window;
  if (!window.init()) {
    std::cout << "Failed to initialize window." << std::endl;
    return 1;
  }
  std::cout << "Window init OK" << std::endl;

  do {
    for (int i = 0; i < 70224; ++i) {
      gb.tick();
    }

    gbeml::u32 *buffer = display.getBuffer();
    if (!window.update(buffer)) {
      break;
    }
  } while (window.waitSync());

  std::cout << "OK" << std::endl;
  return 0;
}
