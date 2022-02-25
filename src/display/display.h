#ifndef GBEMU_DISPLAY_H_
#define GBEMU_DISPLAY_H_

#include <array>

#include "graphics/color.h"
#include "types/types.h"

namespace gbemu {

class Display {
 public:
  virtual ~Display();
  virtual void pushRow(u8 y, const std::array<Color, 160>& pixels) = 0;
  virtual u32 getPixel(u32 i) = 0;
};

}  // namespace gbemu

#endif  // GBEMU_DISPLAY_H_
