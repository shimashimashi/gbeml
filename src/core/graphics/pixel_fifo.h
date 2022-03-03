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
  void pushTileData(u8 low, u8 high);
  Color popPixel();
  u64 size();

  void setPalette(u8 value);
  u8 getPalette() const;

 private:
  Palette palette;
  std::queue<Color> pixels;
};

}  // namespace gbemu

#endif  // GBEMU_PIXEL_FIFO_H_
