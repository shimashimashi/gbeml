#ifndef GBEMU_PIXEL_FIFO_H_
#define GBEMU_PIXEL_FIFO_H_

#include <array>
#include <queue>

#include "core/display/display.h"
#include "core/graphics/color.h"
#include "core/graphics/palette.h"
#include "core/register/register.h"
#include "core/types/types.h"

namespace gbemu {

class PixelFifo {
 public:
  PixelFifo() : palette(0) {}
  void pushLowTileData(u8 data);
  void pushHighTileData(u8 data);
  void popPixels(std::array<Color, 160>* pixels);

  void setScx(u8 value);
  void setPalette(u8 value);

 private:
  Palette palette;

  std::queue<u8> low_pixel_data;
  std::queue<u8> high_pixel_data;

  u8 scx = 0;
  u8 num_unused_pixels = 0;
};

}  // namespace gbemu

#endif  // GBEMU_PIXEL_FIFO_H_
