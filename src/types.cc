
#include "types.h"

namespace gbemu {

u16 concat(u8 h, u8 l) {
  u16 n = h;
  n <<= 8;
  return h + l;
}

}  // namespace gbemu