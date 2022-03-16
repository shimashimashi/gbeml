#include "core/graphics/tile.h"

namespace gbeml {

Tile::Tile(u8 low, u8 high, const Palette& palette) {
  for (i8 i = 7; i >= 0; --i) {
    u8 h = (high >> i) & 1;
    h <<= 1;
    u8 l = (low >> i) & 1;
    u8 color_number = h + l;
    Color color = palette.getColor(color_number);

    colors.push_back(color);
  }
}

Color Tile::getAt(u8 index) const { return colors[index]; }

void Tile::setAt(u8 index, Color color) { colors[index] = color; }

u64 Tile::size() const { return colors.size(); }

void Tile::reverse() { std::reverse(colors.begin(), colors.end()); }

}  // namespace gbeml
