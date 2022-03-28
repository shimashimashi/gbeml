#ifndef GBEML_DISPLAY_IMPL_H_
#define GBEML_DISPLAY_IMPL_H_

#include "core/display/display.h"

namespace gbeml {

class DisplayImpl : public Display {
 public:
  void render(u8 x, u8 y, Color pixel) override;
  u32* getBuffer() override;

 private:
  u32 buffer[160 * 144];
};

}  // namespace gbeml

#endif  // GBEML_DISPLAY_IMPL_H_
