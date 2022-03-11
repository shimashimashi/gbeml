#include "driver/stub/stub_display.h"

#include "glog/logging.h"

namespace gbeml {

void StubDisplay::render(u8 x, u8 y, Color pixel) {
  u32 rgb;
  switch (pixel) {
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

  buffer[y * 160 + x] = rgb;
}

u32* StubDisplay::getBuffer() { return buffer; }

}  // namespace gbeml
