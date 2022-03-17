#ifndef GBEML_LCDC_H_
#define GBEML_LCDC_H_

#include "core/graphics/sprite.h"
#include "core/register/register.h"
#include "core/types/types.h"

namespace gbeml {

class Lcdc {
 public:
  Lcdc(u8 flags_) : flags(flags_) {}

  u8 read() const;
  void write(u8 value);

  bool isLcdEnabled() const;
  bool isWindowEnabled() const;
  bool isSpriteEnabled() const;
  bool isBackgroundEnabled() const;

  u16 getWindowTileMapAddress(u16 offset) const;
  u16 getBackgroundTileDataAddress(u8 tile_number) const;
  u16 getBackgroundTileMapAddress(u16 offset) const;
  SpriteSize spriteSize() const;

 private:
  Register flags;
};

}  // namespace gbeml

#endif  // GBEML_LCDC_H_
