#include "cpu/opcode.h"

#include <cstdio>

namespace gbemu {

u8 Opcode::get() const { return value; }

bool Opcode::match(std::string pattern) const {
  for (i64 i = 0; i < 8; ++i) {
    char actual = value >> i & 1 ? '1' : '0';
    char expected = pattern.back();
    pattern.pop_back();
    if (expected != '0' && expected != '1') continue;

    if (actual != expected) return false;
  }
  return true;
}

u8 Opcode::slice(u8 from, u8 to) const {
  u8 code = value;
  code >>= from;

  u8 mask = 1;
  while (to > from) {
    mask <<= 1;
    mask |= 1;
    to--;
  }

  return code & mask;
}

}  // namespace gbemu
