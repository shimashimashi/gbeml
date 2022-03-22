#ifndef GBEML_GAMEBOY_H_
#define GBEML_GAMEBOY_H_

#include <string>

#include "core/bus/bus.h"
#include "core/cpu/cpu.h"
#include "core/display/display.h"
#include "core/graphics/ppu.h"
#include "core/interrupt/interrupt_controller.h"
#include "core/joypad/joypad.h"
#include "core/memory/mbc.h"
#include "core/memory/ram.h"
#include "core/memory/rom.h"
#include "core/timer/timer.h"

namespace gbeml {

class GameBoy {
 public:
  GameBoy(Display* display_, i32 breakpoint_)
      : display(display_), breakpoint(breakpoint_) {}
  void tick();
  bool init(const std::string& filename);
  Display* getDisplay() const;
  void press(JoypadButton button);
  void release(JoypadButton button);

 private:
  Display* display;
  Bus* bus;
  Cpu* cpu;
  Ppu* ppu;
  Mbc* mbc;
  Rom* rom;
  Ram* hram;
  Ram* wram;
  Ram* vram;
  Ram* oam;
  InterruptController* ic;
  Timer* timer;
  Joypad* joypad;

  i32 breakpoint;
};

}  // namespace gbeml

#endif  // GBEML_GAMEBOY_H_
