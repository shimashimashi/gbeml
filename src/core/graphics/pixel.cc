#include "core/graphics/pixel.h"

namespace gbeml {

Color BackgroundPixel::getColor() const { return color; }

Color SpritePixel::getColor() const { return color; }

bool SpritePixel::isBackgroundOverSprite() const {
  return is_background_over_sprite;
}

}  // namespace gbeml
