#include "register.h"

namespace gbemu {

u16 Register::get() { return value; }

u8 Register::getLow() { return value & 0xff; }

u8 Register::getHigh() { return value >> 8; }

bool Register::getAt(u8 i) { return (value >> i & 1) == 1; }

void Register::set(u16 n) { value = n; }

void Register::setLow(u8 n) { value = value & 0xff00 | n; }

void Register::setHigh(u8 n) {
  u16 h = static_cast<u16>(n) << 8;
  u16 l = value & 0x00ff;
  value = h | l;
}

void Register::setAt(u8 i, bool b) {
  u8 n = b;
  n <<= i;
  value |= n;
}

void Register::increment() { value++; }

void Register::decrement() { value--; }

}  // namespace gbemu
