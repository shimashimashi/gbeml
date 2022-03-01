#ifndef GBEMU_PALETTE_H_
#define GBEMU_PALETTE_H_

#include "core/graphics/color.h"
#include "core/register/register.h"
#include "core/types/types.h"

namespace gbemu {

class Palette {
 public:
  Palette(u8 data_) : data(data_) {}

  u8 read() const;
  void write(u8 value);
  Color getColor(u8 color_id);

 private:
  Register data;
};

}  // namespace gbemu

#endif  // GBEMU_PALETTE_H_
