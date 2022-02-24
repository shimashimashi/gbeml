#ifndef GBEMU_GAMEBOY_H_
#define GBEMU_GAMEBOY_H_

#include <string>

#include "bus/bus.h"
#include "bus/bus_impl.h"
#include "cpu/cpu.h"
#include "graphics/display.h"
#include "graphics/ppu.h"
#include "interrupt/interrupt_controller.h"
#include "memory/mbc.h"
#include "memory/ram.h"
#include "memory/ram_impl.h"
#include "memory/rom.h"

namespace gbemu {

class GameBoy {
 public:
  void tick();
  void init(const std::string& filename);

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
