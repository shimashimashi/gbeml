#include "core/graphics/pixel_fifo.h"

namespace gbeml {

void PixelFifo::pushTileData(u8 low, u8 high) {
  for (i8 i = 7; i >= 0; --i) {
    u8 h = (high >> i) & 1;
    h <<= 1;
    u8 l = (low >> i) & 1;
    u8 color_number = h + l;
    Color color = palette.getColor(color_number);

    pixels.push(color);
  }
}

void PixelFifo::setPalette(u8 value) { palette.write(value); }

u8 PixelFifo::getPalette() const { return palette.read(); }

Color PixelFifo::popPixel() {
  Color pixel = pixels.front();
  pixels.pop();
  return pixel;
}

u64 PixelFifo::size() { return pixels.size(); }

}  // namespace gbeml
