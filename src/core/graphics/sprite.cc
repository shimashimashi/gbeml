#include "core/graphics/sprite.h"

namespace gbeml {

u8 Sprite::getY() const { return y; }

u8 Sprite::getX() const { return x; }

u8 Sprite::getTileIndex() const { return tile_index; }

bool Sprite::isBackgroundOverSprite() const { return flags.getAt(7); }

bool Sprite::flipY() const { return flags.getAt(6); }

bool Sprite::flipX() const { return flags.getAt(5); }

bool Sprite::getPaletteNumber() const { return flags.getAt(4); }

u8 Sprite::getPixelIndex(u8 current_x) const { return current_x - (x - 8); }

bool Sprite::isVisibleVertically(u8 ly, SpriteSize size) const {
  if (x == 0) {
    return false;
  }

  if (ly < y - 16) {
    return false;
  }

  switch (size) {
    case SpriteSize::Tall:
      return ly < y;
    case SpriteSize::Short:
      return ly < y - 8;
  }
}

bool Sprite::isVisibleHorizontally(u8 current_x) const {
  return current_x >= x - 8 && current_x < x;
}

u16 Sprite::getTileDataAddress(u8 ly, SpriteSize size) const {
  switch (size) {
    case SpriteSize::Tall: {
      u16 base = (tile_index & 0xfe) * 16;
      if (flipY()) {
        return base + 2 * (y - 1 - ly);
      } else {
        return base + 2 * (ly - (y - 16));
      }
    }
    case SpriteSize::Short: {
      u16 base = tile_index * 16;
      if (flipY()) {
        return base + 2 * (y - 9 - ly);
      } else {
        return base + 2 * (ly - (y - 16));
      }
    }
  }
}

bool Sprite::isDrawn() const { return drawn; }

void Sprite::setDrawn() { drawn = true; }

}  // namespace gbeml
