#include "core/types/types.h"

namespace gbemu {

u16 concat(u8 h, u8 l) {
  u16 n = h;
  n <<= 8;
  n |= l;
  return n;
}

}  // namespace gbemu
