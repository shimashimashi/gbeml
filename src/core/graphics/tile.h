#ifndef GBEML_TILE_H_
#define GBEML_TILE_H_

#include <vector>

#include "core/graphics/color.h"
#include "core/graphics/palette.h"

namespace gbeml {

class Tile {
 public:
  Tile(u8 low_, u8 high_, const Palette& palette_)
      : low(low_), high(high_), palette(palette_) {}

  Color getAt(u8 index) const;

 private:
  u8 low;
  u8 high;
  const Palette& palette;
};

}  // namespace gbeml

#endif  // GBEML_TILE_H_
