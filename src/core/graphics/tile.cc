#include "core/graphics/tile.h"

namespace gbeml {

Color Tile::getAt(u8 index) const {
  u8 h = (high >> (7 - index)) & 1;
  u8 l = (low >> (7 - index)) & 1;
  return palette.getColor((h << 1) | l);
}

}  // namespace gbeml
