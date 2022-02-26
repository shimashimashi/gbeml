#ifndef GBEMU_GAMEBOY_H_
#define GBEMU_GAMEBOY_H_

#include <string>

#include "bus/bus.h"
#include "cpu/cpu.h"
#include "display/display.h"
#include "graphics/ppu.h"
#include "interrupt/interrupt_controller.h"
#include "memory/mbc.h"
#include "memory/ram.h"
#include "memory/rom.h"

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
