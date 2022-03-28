#ifndef GBEML_DISPLAY_H_
#define GBEML_DISPLAY_H_

#include "core/graphics/color.h"
#include "core/types/types.h"

namespace gbeml {

class Display {
 public:
  virtual ~Display() {}
  virtual void render(u8 x, u8 y, Color pixel) = 0;
  virtual u32* getBuffer() = 0;
};

}  // namespace gbeml

#endif  // GBEML_DISPLAY_H_
