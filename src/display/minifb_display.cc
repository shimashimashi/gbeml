#include "display/minifb_display.h"

#include "glog/logging.h"

namespace gbemu {

void MiniFbDisplay::pushRow(u8 y, const std::array<Color, 160>& pixels) {
  DCHECK(y < 144);

  for (u8 i = 0; i < 160; ++i) {
    u32 rgb;
    switch (pixels[i]) {
      case Color::White:
        rgb = 0xffffff;
        break;
      case Color::LightGray:
        rgb = 0xd3d3d3;
        break;
      case Color::DarkGray:
        rgb = 0xa9a9a9;
        break;
      case Color::Black:
        rgb = 0x000000;
        break;
    }

    buffer[y * 160 + i] = rgb;
  }
}

// u32 MiniFbDisplay::getPixel(u32 i) { return buffer[i]; }
u32* MiniFbDisplay::getBuffer() { return buffer; }

}  // namespace gbemu
