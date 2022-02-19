#ifndef GBEMU_BUS_H_
#define GBEMU_BUS_H_

#include <array>

#include "bus.h"

namespace gbemu {

class Mmap : public Bus {
 public:
  Mmap(Mbc* mbc_) : mbc(mbc_) {}
  u8 read(u16 addr) const;
  void write(u16 addr, u8 value);

 private:
  Mbc* mbc;
  std::array<u8, 8 * 1024> wram;
  std::array<u8, 128> hram;
};

}  // namespace gbemu

#endif  // GBEMU_BUS_H_
