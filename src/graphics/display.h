#ifndef GBEMU_DISPLAY_H_
#define GBEMU_DISPLAY_H_

#include <array>

#include "graphics/color.h"
#include "types/types.h"

namespace gbemu {

class Display {
 public:
  virtual ~Display();
  virtual void renderRow(u8 y, std::array<Color, 160> pixels) = 0;
};

}  // namespace gbemu

#endif  // GBEMU_DISPLAY_H_
