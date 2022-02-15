#ifndef GBEMU_BUS_H_
#define GBEMU_BUS_H_

#include <array>

#include "mbc.h"

namespace gbemu {

class Bus {
 public:
  Bus(Mbc* mbc_) : mbc(mbc_) {}
  void tick();
  u8 read(u16 addr) const;
  void write(u16 addr, u8 value);

 private:
  Mbc* mbc;
  std::array<u8, 8 * 1024> wram;
  std::array<u8, 128> hram;
};

}  // namespace gbemu

#endif  // GBEMU_BUS_H_
