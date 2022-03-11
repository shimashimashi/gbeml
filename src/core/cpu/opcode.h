#ifndef GBEML_OPCODE_H_
#define GBEML_OPCODE_H_

#include <string>

#include "core/register/register.h"
#include "core/types/types.h"

namespace gbeml {

class Opcode {
 public:
  Opcode(u8 code) : value(code) {}

  u8 get() const;
  u8 slice(u8 from, u8 to) const;

 private:
  Register value;
};

}  // namespace gbeml

#endif  // GBEML_OPCODE_H_
