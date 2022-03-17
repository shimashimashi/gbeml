#ifndef GBEML_SPRITE_H_
#define GBEML_SPRITE_H_

#include "core/register/register.h"
#include "core/types/types.h"

namespace gbeml {

enum class SpriteSize { Short, Tall };

class Sprite {
 public:
  Sprite(u8 y_, u8 x_, u8 tile_index_, u8 flags_)
      : y(y_), x(x_), tile_index(tile_index_), flags(flags_), drawn(false) {}

  u8 getY() const;
  u8 getX() const;
  u8 getTileIndex() const;
  u8 getPixelIndex(u8 current_x) const;
  bool isBackgroundOverSprite() const;
  bool flipY() const;
  bool flipX() const;
  bool getPaletteNumber() const;

  bool isVisibleVertically(u8 ly, SpriteSize size) const;
  bool isVisibleHorizontally(u8 current_x) const;
  u16 getTileDataAddress(u8 ly, SpriteSize size) const;

  bool isDrawn() const;
  void setDrawn();

 private:
  u8 y;
  u8 x;
  u8 tile_index;
  Register flags;
  bool drawn;
};

}  // namespace gbeml

#endif  // GBEML_SPRITE_H_
