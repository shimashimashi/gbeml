#include "core/graphics/lcdc.h"

namespace gbeml {

u8 Lcdc::read() const { return flags.get(); }

void Lcdc::write(u8 value) { return flags.set(value); }

bool Lcdc::isLcdEnabled() const { return flags.getAt(7); }

u16 Lcdc::getWindowTileMapAddress(u16 offset) const {
  u16 base = flags.getAt(6) ? 0x1c00 : 0x1800;
  return base + offset;
}

bool Lcdc::isWindowEnabled() const { return flags.getAt(5); }

u16 Lcdc::getBackgroundTileDataAddress(u8 tile_number) const {
  if (flags.getAt(4)) {
    return tile_number * 16;
  } else {
    return static_cast<u16>(0x1000 + static_cast<i8>(tile_number) * 16);
  }
}

u16 Lcdc::getBackgroundTileMapAddress(u16 offset) const {
  u16 base = flags.getAt(3) ? 0x1c00 : 0x1800;
  return base + offset;
}

SpriteSize Lcdc::spriteSize() const {
  return flags.getAt(2) ? SpriteSize::Tall : SpriteSize::Short;
}

bool Lcdc::isSpriteEnabled() const { return flags.getAt(1); }

bool Lcdc::isBackgroundEnabled() const { return flags.getAt(0); }

}  // namespace gbeml
