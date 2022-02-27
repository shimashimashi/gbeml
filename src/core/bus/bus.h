#ifndef GBEMU_BUS_H_
#define GBEMU_BUS_H_

#include "core/types/types.h"

namespace gbemu {

class Bus {
 public:
  virtual ~Bus();
  virtual u8 read(u16 addr) const = 0;
  virtual void write(u16 addr, u8 value) = 0;
  virtual void tick() = 0;
};

}  // namespace gbemu

#endif  // GBEMU_BUS_H_
