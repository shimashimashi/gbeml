#include "core/graphics/pixel_fifo.h"

namespace gbemu {

void PixelFifo::pushLowTileData(u8 data) { low_pixel_data.push(data); }

void PixelFifo::pushHighTileData(u8 data) { high_pixel_data.push(data); }

void PixelFifo::setScx(u8 value) {
  scx = value;
  num_unused_pixels = scx % 8;
}

void PixelFifo::setPalette(u8 value) { palette.write(value); }

void PixelFifo::popPixels(std::array<Color, 160>* pixels) {
  u8 x = 0;

  while (x < 160) {
    u8 high = high_pixel_data.front();
    high_pixel_data.pop();

    u8 low = low_pixel_data.front();
    low_pixel_data.pop();

    for (i8 i = 7; i >= 0; --i) {
      if (num_unused_pixels > 0) {
        num_unused_pixels--;
        continue;
      }

      u8 h = (high >> i) & 1;
      h <<= 1;
      u8 l = (low >> i) & 1;
      u8 color_number = h + l;
      Color color = palette.getColor(color_number);

      if (x < 160) {
        (*pixels)[x++] = color;
      }
    }
  }

  while (!high_pixel_data.empty()) {
    high_pixel_data.pop();
    low_pixel_data.pop();
  }
}

}  // namespace gbemu
