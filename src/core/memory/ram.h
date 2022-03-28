#ifndef GBEML_RAM_H_
#define GBEML_RAM_H_

#include "core/types/types.h"

namespace gbeml {

class Ram {
 public:
  virtual ~Ram() {}
  virtual u8 read(u16 addr) const = 0;
  virtual void write(u16 addr, u8 value) = 0;
};

}  // namespace gbeml

#endif  // GBEML_RAM_H_
