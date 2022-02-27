#ifndef GBEMU_OPCODE_H_
#define GBEMU_OPCODE_H_

#include <string>

#include "core/register/register.h"
#include "core/types/types.h"

namespace gbemu {

class Opcode {
 public:
  Opcode(u8 code) : value(code) {}

  u8 get() const;
  bool match(const std::string& pattern) const;
  u8 slice(u8 from, u8 to) const;

 private:
  Register value;
};

}  // namespace gbemu

#endif  // GBEMU_OPCODE_H_
