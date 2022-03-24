#include "core/register/register.h"

#include <cstdio>

namespace gbeml {

u8 Register::get() const { return value; }

bool Register::getAt(u8 i) const { return value >> i & 1; }

u8 Register::slice(u8 from, u8 to) const {
  u8 mask = 0xff >> (7 - to);
  return (value & mask) >> from;
}

void Register::set(u8 n) { value = n; }

void Register::setAt(u8 i, bool b) {
  u8 n = 1 << i;
  if (b) {
    value |= n;
  } else {
    value &= ~n;
  }
}

void Register::increment() { set(get() + 1); }

void Register::decrement() { set(get() - 1); }

u16 RegisterPair::get() const { return concat(high.get(), low.get()); }

void RegisterPair::set(u16 n) {
  high.set(n >> 8);
  low.set(n & 0xff);
}

Register* RegisterPair::getHigh() { return &high; }

Register* RegisterPair::getLow() { return &low; }

void RegisterPair::increment() { set(get() + 1); }

void RegisterPair::decrement() { set(get() - 1); }

}  // namespace gbeml
