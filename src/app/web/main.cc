#include <iostream>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "core/gameboy.h"
#include "driver/sdl/sdl_window.h"

gbeml::SdlWindow *window;

void mainLoop() {
  SDL_Event event;
  SDL_PollEvent(&event);
  window->runLoop(event);
}

int main(int argc, char *argv[]) {
  gbeml::GameBoy gb(-1);
  if (!gb.init("/rom/PLUTOS_CORNER.gb")) {
    std::cerr << "Failed to initialize gb." << std::endl;
    return 1;
  }
  std::cout << "GB init OK" << std::endl;

  window = new gbeml::SdlWindow(&gb);
  if (!window->init()) {
    std::cerr << "Failed to initialize window." << std::endl;
    return 1;
  }
  std::cout << "Window init OK" << std::endl;

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(&mainLoop, 60, 1);
#endif

  std::cout << "OK" << std::endl;
  return 0;
}
