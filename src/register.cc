#include "register.h"

#include <cstdio>

namespace gbemu {

u16 Register::get() { return value; }

u8 Register::getLow() { return value & 0xff; }

u8 Register::getHigh() { return value >> 8; }

bool Register::getAt(u8 i) { return (value >> i & 1) == 1; }

void Register::set(u16 n) { value = n; }

void Register::setLow(u8 n) { value = (value & 0xff00) | n; }

void Register::setHigh(u8 n) {
  u16 h = n;
  h <<= 8;
  u16 l = value & 0x00ff;
  value = h + l;
}

void Register::setAt(u8 i, bool b) {
  u8 n = 1;
  n <<= i;
  if (b) {
    value |= n;
  } else {
    value &= ~n;
  }
}

void Register::increment() { value++; }

void Register::decrement() { value--; }

}  // namespace gbemu
