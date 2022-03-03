#ifndef GBEMU_MINIFB_DISPLAY_H_
#define GBEMU_MINIFB_DISPLAY_H_

#include <array>

#include "core/display/display.h"
#include "core/graphics/color.h"
#include "core/types/types.h"

namespace gbemu {

class MiniFbDisplay : public Display {
 public:
  MiniFbDisplay() {}
  void render(u8 x, u8 y, Color pixel) override;
  u32* getBuffer();

 private:
  u32 buffer[160 * 144];
};

}  // namespace gbemu

#endif  // GBEMU_MINIFB_DISPLAY_H_
