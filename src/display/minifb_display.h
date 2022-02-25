#ifndef GBEMU_MINIFB_DISPLAY_H_
#define GBEMU_MINIFB_DISPLAY_H_

#include <array>

#include "display/display.h"
#include "graphics/color.h"
#include "types/types.h"

namespace gbemu {

class MiniFbDisplay : public Display {
 public:
  MiniFbDisplay() {}
  void pushRow(u8 y, const std::array<Color, 160>& pixels) override;
  u32 getPixel(u32 i) override;

 private:
  std::array<u32, 160 * 144> buffer;
};

}  // namespace gbemu

#endif  // GBEMU_MINIFB_DISPLAY_H_
