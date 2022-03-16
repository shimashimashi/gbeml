#ifndef GBEML_TILE_H_
#define GBEML_TILE_H_

#include <vector>

#include "core/graphics/color.h"
#include "core/graphics/palette.h"

namespace gbeml {

class Tile {
 public:
  Tile(u8 low, u8 high, const Palette& palette);

  Color getAt(u8 index) const;
  void setAt(u8 index, Color color);
  void reverse();
  u64 size() const;

 private:
  std::vector<Color> colors;
};

}  // namespace gbeml

#endif  // GBEML_TILE_H_
