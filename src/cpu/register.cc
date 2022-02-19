#include "cpu/register.h"

#include <cstdio>

namespace gbemu {

u8 Register::get() { return value; }

bool Register::getAt(u8 i) { return value >> i & 1; }

void Register::set(u8 n) { value = n; }

void Register::setAt(u8 i, bool b) {
  u8 n = 1;
  n <<= i;
  if (b) {
    value |= n;
  } else {
    value &= ~n;
  }
}

void Register::increment() { set(get() + 1); }

void Register::decrement() { set(get() - 1); }

u16 RegisterPair::get() { return concat(high->get(), low->get()); }

void RegisterPair::set(u16 n) {
  high->set(n >> 8);
  low->set(n & 0xff);
}

Register* RegisterPair::getHigh() { return high; }

Register* RegisterPair::getLow() { return low; }

void RegisterPair::increment() { set(get() + 1); }

void RegisterPair::decrement() { set(get() - 1); }

}  // namespace gbemu
