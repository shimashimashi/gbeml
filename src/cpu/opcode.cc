#include "cpu/opcode.h"

#include <cstdio>

namespace gbemu {

u8 Opcode::get() const { return value.get(); }

bool Opcode::match(const std::string& pattern) const {
  for (u8 i = 0; i < 8; ++i) {
    char expected = pattern[7 - i];
    if (expected != '0' && expected != '1') continue;

    char actual = value.getAt(i) ? '1' : '0';
    if (actual != expected) return false;
  }
  return true;
}

u8 Opcode::slice(u8 from, u8 to) const { return value.slice(from, to); }

}  // namespace gbemu
