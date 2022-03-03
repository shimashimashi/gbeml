#ifndef GBEMU_DISPLAY_H_
#define GBEMU_DISPLAY_H_

#include <array>

#include "core/graphics/color.h"
#include "core/types/types.h"

namespace gbemu {

class Display {
 public:
  virtual ~Display();
  virtual void render(u8 x, u8 y, Color pixel) = 0;
};

}  // namespace gbemu

#endif  // GBEMU_DISPLAY_H_
