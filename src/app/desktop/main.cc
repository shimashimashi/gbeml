#include <gflags/gflags.h>

#include <cmath>
#include <iostream>

#include "core/gameboy.h"
#include "core/log/logging.h"
#include "driver/sdl/sdl_window.h"

DEFINE_string(filename, "", "Rom filename");
DEFINE_int32(breakpoint, -1, "PC at breakpoint");
DEFINE_bool(stub, false, "Use stub display");
DEFINE_bool(sdl, false, "Use sdl display");
DEFINE_int32(n_frame, -1, "Number of frames to update");

void runSdl(gbeml::GameBoy *gb) {
  gbeml::SdlWindow window(gb);
  if (!window.init()) {
    std::cerr << "Failed to initialize window." << std::endl;
    return;
  }
  std::cout << "Window init OK" << std::endl;

  while (true) {
    Uint64 start = SDL_GetPerformanceCounter();

    SDL_Event event;
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT) {
      break;
    }
    window.runLoop(event);

    Uint64 end = SDL_GetPerformanceCounter();
    float elapsedMS =
        (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
    if (elapsedMS < 16.666f) {
      SDL_Delay(floor(16.666f - elapsedMS));
    }
  }
}

void runStub(gbeml::GameBoy *gb) {
  int n = FLAGS_n_frame;
  while (n > 0) {
    for (int i = 0; i < 70224; ++i) {
      gb->tick();
    }
    n--;
  }
}

int main(int argc, char *argv[]) {
  google::InitGoogleLogging(argv[0]);
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  if (FLAGS_filename.empty()) {
    std::cerr << "Rom filename is empty." << std::endl;
    return 1;
  }

  int breakpoint;
  std::stringstream ss;
  ss << std::hex << FLAGS_breakpoint;
  ss >> breakpoint;

  gbeml::GameBoy gb(breakpoint);
  if (!gb.init(FLAGS_filename)) {
    std::cerr << "Failed to initialize gb." << std::endl;
    return 1;
  }
  std::cout << "GB init OK" << std::endl;

  if (FLAGS_stub) {
    runStub(&gb);
  } else {
    runSdl(&gb);
  }

  std::cout << "OK" << std::endl;
  return 0;
}
