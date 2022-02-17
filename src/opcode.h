#ifndef GBEMU_OPCODE_H_
#define GBEMU_OPCODE_H_

#include <string>

#include "types.h"

namespace gbemu {

class Opcode {
 public:
  Opcode(u8 code) : value(code) {}

  u8 get() const;
  bool match(std::string pattern) const;
  u8 slice(u8 from, u8 to) const;

 private:
  u8 value;
};

}  // namespace gbemu

#endif  // GBEMU_OPCODE_H_
