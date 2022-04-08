#include <iostream>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "core/gameboy.h"
#include "core/joypad/joypad.h"
#include "driver/sdl/sdl_window.h"

gbeml::SdlWindow *window;
gbeml::GameBoy *gb;

extern "C" {

void pressButton(int id) {
  switch (id) {
    case 0:
      gb->press(gbeml::JoypadButton::A);
      break;
    case 1:
      gb->press(gbeml::JoypadButton::B);
      break;
    case 2:
      gb->press(gbeml::JoypadButton::Start);
      break;
    case 3:
      gb->press(gbeml::JoypadButton::Select);
      break;
    case 4:
      gb->press(gbeml::JoypadButton::Up);
      break;
    case 5:
      gb->press(gbeml::JoypadButton::Down);
      break;
    case 6:
      gb->press(gbeml::JoypadButton::Left);
      break;
    case 7:
      gb->press(gbeml::JoypadButton::Right);
      break;
    default:
      break;
  }
}

void releaseButton(int id) {
  switch (id) {
    case 0:
      gb->release(gbeml::JoypadButton::A);
      break;
    case 1:
      gb->release(gbeml::JoypadButton::B);
      break;
    case 2:
      gb->release(gbeml::JoypadButton::Start);
      break;
    case 3:
      gb->release(gbeml::JoypadButton::Select);
      break;
    case 4:
      gb->release(gbeml::JoypadButton::Up);
      break;
    case 5:
      gb->release(gbeml::JoypadButton::Down);
      break;
    case 6:
      gb->release(gbeml::JoypadButton::Left);
      break;
    case 7:
      gb->release(gbeml::JoypadButton::Right);
      break;
    default:
      break;
  }
}
}

void mainLoop() { window->runLoop(); }

int main(int argc, char *argv[]) {
  gb = new gbeml::GameBoy(-1);
  if (!gb->init("/rom/PLUTOS_CORNER.gb")) {
    std::cerr << "Failed to initialize gb." << std::endl;
    return 1;
  }
  std::cout << "GB init OK" << std::endl;

  window = new gbeml::SdlWindow(gb);
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
