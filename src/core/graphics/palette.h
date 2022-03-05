#ifndef GBEML_PALETTE_H_
#define GBEML_PALETTE_H_

#include "core/graphics/color.h"
#include "core/register/register.h"
#include "core/types/types.h"

namespace gbeml {

class Palette {
 public:
  Palette(u8 data_) : data(data_) {}

  u8 read() const;
  void write(u8 value);
  Color getColor(u8 color_id);

 private:
  Register data;
};

}  // namespace gbeml

#endif  // GBEML_PALETTE_H_
