#ifndef GBEMU_DOT_H_
#define GBEMU_DOT_H_

#include "core/graphics/mode.h"
#include "core/types/types.h"

namespace gbemu {
class Dot {
 public:
  u8 getScy() const;
  u8 getScx() const;
  u8 getLy() const;
  u8 getLyc() const;
  u8 getLx() const;
  u8 getWy() const;
  u8 getWx() const;

  void setScy(u8 value);
  void setScx(u8 value);
  void setLy(u8 value);
  void setLyc(u8 value);
  void setWy(u8 value);
  void setWx(u8 value);

  void moveNext();

  PpuMode getMode() const;
  bool isLycEqualsLy() const;

  u16 getTileMapAddressOffset();
  u16 getLowTileDataAddressOffset();
  u16 getHighTileDataAddressOffset();

  bool discardsCurrentPixel();

 private:
  u8 scy = 0;
  u8 scx = 0;
  u8 ly = 0;
  u8 lyc = 0;
  u16 lx = 0;
  u8 wy = 0;
  u8 wx = 0;

  u8 num_unused_pixels = 0;
};
}  // namespace gbemu

#endif  // GBEMU_DOT_H_
