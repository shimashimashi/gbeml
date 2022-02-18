#include "register.h"

#include <cstdio>

namespace gbemu {

u8 HalfRegister::get() { return value; }

bool HalfRegister::getAt(u8 i) { return value >> i & 1; }

void HalfRegister::set(u8 n) { value = n; }

void HalfRegister::setAt(u8 i, bool b) {
  u8 n = 1;
  n <<= i;
  if (b) {
    value |= n;
  } else {
    value &= ~n;
  }
}

void HalfRegister::increment() { set(get() + 1); }

void HalfRegister::decrement() { set(get() - 1); }

u16 Register::get() { return concat(high->get(), low->get()); }

void Register::set(u16 n) {
  high->set(n >> 8);
  low->set(n & 0xff);
}

void Register::increment() { set(get() + 1); }

void Register::decrement() { set(get() - 1); }

}  // namespace gbemu
