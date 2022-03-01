#include "core/graphics/palette.h"
#include "glog/logging.h"

namespace gbemu {

u8 Palette::read() const { return data.get(); }

void Palette::write(u8 value) { return data.set(value); }

Color Palette::getColor(u8 color_id) {
  u8 color = 0;
  switch (color_id) {
    case 0:
      color = data.slice(0, 1);
      break;
    case 1:
      color = data.slice(2, 3);
      break;
    case 2:
      color = data.slice(4, 5);
      break;
    case 3:
      color = data.slice(6, 7);
      break;
    default:
      DCHECK(false);
      break;
  }

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
}  // namespace gbemu
