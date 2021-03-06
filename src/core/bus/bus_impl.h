#ifndef GBEML_BUS_IMPL_H_
#define GBEML_BUS_IMPL_H_

#include "core/bus/bus.h"
#include "core/graphics/ppu.h"
#include "core/interrupt/interrupt_controller.h"
#include "core/joypad/joypad.h"
#include "core/memory/mbc.h"
#include "core/memory/ram.h"
#include "core/timer/timer.h"
#include "core/types/types.h"

namespace gbeml {

enum class BusMode { Normal, Dma };

class BusImpl : public Bus {
 public:
  BusImpl(Mbc* mbc_, Ram* wram_, Ram* hram_, Ppu* ppu_, Timer* timer_,
          InterruptController* ic_, Joypad* joypad_)
      : mbc(mbc_),
        wram(wram_),
        hram(hram_),
        ppu(ppu_),
        timer(timer_),
        ic(ic_),
        joypad(joypad_) {}

  u8 read(u16 addr) const override;
  void write(u16 addr, u8 value) override;
  void tick() override;

 private:
  Mbc* mbc;
  Ram* wram;
  Ram* hram;
  Ppu* ppu;
  Timer* timer;
  InterruptController* ic;
  Joypad* joypad;

  u32 stalls = 0;
  u16 dma_source_address;
  BusMode mode = BusMode::Normal;

  void enterDma(u8 source);

  void transfer();
};

}  // namespace gbeml

#endif  // GBEML_BUS_IMPL_H_
