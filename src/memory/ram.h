#ifndef GBEMU_RAM_H_
#define GBEMU_RAM_H_

#include "types/types.h"

namespace gbemu {

class Ram {
 public:
  virtual ~Ram();
  virtual u8 read(u16 addr) const = 0;
  virtual void write(u16 addr, u8 value) = 0;
};

}  // namespace gbemu

#endif  // GBEMU_RAM_H_
