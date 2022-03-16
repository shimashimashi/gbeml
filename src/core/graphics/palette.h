#ifndef GBEML_PALETTE_H_
#define GBEML_PALETTE_H_

#include "core/graphics/color.h"
#include "core/register/register.h"
#include "core/types/types.h"

namespace gbeml {

class Palette {
 public:
  virtual ~Palette() {}

  virtual u8 read() const = 0;
  virtual void write(u8 value) = 0;
  virtual Color getColor(u8 color_index) const = 0;
};

class BackgroundPalette : public Palette {
 public:
  BackgroundPalette(u8 data_) : data(data_) {}

  virtual u8 read() const override;
  virtual void write(u8 value) override;
  virtual Color getColor(u8 color_index) const override;

 private:
  Register data;
};

class SpritePalette : public Palette {
 public:
  SpritePalette(u8 data_) : data(data_) {}

  virtual u8 read() const override;
  virtual void write(u8 value) override;
  virtual Color getColor(u8 color_index) const override;

 private:
  Register data;
};

}  // namespace gbeml

#endif  // GBEML_PALETTE_H_
