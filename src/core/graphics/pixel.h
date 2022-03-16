#ifndef GBEML_PIXEL_H_
#define GBEML_PIXEL_H_

#include "core/graphics/color.h"

namespace gbeml {

class BackgroundPixel {
 public:
  BackgroundPixel(Color color_) : color(color_) {}
  Color getColor() const;

 private:
  Color color;
};

class SpritePixel {
 public:
  SpritePixel(Color color_, bool is_background_over_sprite_)
      : color(color_), is_background_over_sprite(is_background_over_sprite_) {}
  Color getColor() const;
  bool isBackgroundOverSprite() const;

 private:
  Color color;
  bool is_background_over_sprite;
};

}  // namespace gbeml

#endif  // GBEML_PIXEL_H_
