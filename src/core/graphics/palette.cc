#include "core/graphics/palette.h"

#include "core/log/logging.h"

namespace gbeml {

u8 getColorId(u8 color_index, const Register& r) {
  switch (color_index) {
    case 0:
      return r.slice(0, 1);
    case 1:
      return r.slice(2, 3);
    case 2:
      return r.slice(4, 5);
    case 3:
      return r.slice(6, 7);
    default:
      DCHECK(false);
      return 0;
  }
}

u8 BackgroundPalette::read() const { return data.get(); }

void BackgroundPalette::write(u8 value) { return data.set(value); }

Color BackgroundPalette::getColor(u8 color_index) const {
  u8 color = getColorId(color_index, data);

  switch (color) {
    case 0:
      return Color::White;
    case 1:
      return Color::LightGray;
    case 2:
      return Color::DarkGray;
    case 3:
      return Color::Black;
    default:
      DCHECK(false);
      return Color::White;
  }
}

u8 SpritePalette::read() const { return data.get(); }

void SpritePalette::write(u8 value) { return data.set(value); }

Color SpritePalette::getColor(u8 color_index) const {
  if (color_index == 0) {
    return Color::Transparent;
  }

  u8 color = getColorId(color_index, data);

  switch (color) {
    case 0:
      return Color::White;
    case 1:
      return Color::LightGray;
    case 2:
      return Color::DarkGray;
    case 3:
      return Color::Black;
    default:
      DCHECK(false);
      return Color::White;
  }
}

}  // namespace gbeml
