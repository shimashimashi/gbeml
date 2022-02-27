#ifndef GBEMU_GAMEBOY_H_
#define GBEMU_GAMEBOY_H_

#include <string>

#include "core/bus/bus.h"
#include "core/cpu/cpu.h"
#include "core/display/display.h"
#include "core/graphics/ppu.h"
#include "core/interrupt/interrupt_controller.h"
#include "core/memory/mbc.h"
#include "core/memory/ram.h"
#include "core/memory/rom.h"

namespace gbemu {

class GameBoy {
 public:
  GameBoy(Display* display_) : display(display_) {}
  void tick();
  bool init(const std::string& filename);

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
};

}  // namespace gbemu

#endif  // GBEMU_GAMEBOY_H_
