#include "core/graphics/dot.h"

namespace gbemu {

void Dot::moveNext() {
  if (++lx == 456) {
    lx = 0;
    if (++ly == 154) {
      ly = 0;
    }
  }
}

bool Dot::isLycEqualsLy() const { return ly == lyc; }

PpuMode Dot::getMode() const {
  if (ly >= 144) {
    return PpuMode::VBlank;
  }

  if (lx < 80) {
    return PpuMode::OamScan;
  } else if (lx < 252) {
    return PpuMode::Drawing;
  } else {
    return PpuMode::HBlank;
  }
}

u16 Dot::getTileMapAddressOffset() {
  u8 x = ((scx + lx - 80) & 0xff) / 8;
  u8 y = ((scy + ly) & 0xff) / 8;
  u16 offset = (x + y * 32) & 0x3ff;
  return offset;
}

u16 Dot::getLowTileDataAddressOffset() { return 2 * ((ly + scy) % 8); }

u16 Dot::getHighTileDataAddressOffset() { return 2 * ((ly + scy) % 8) + 1; }

u8 Dot::getScy() const { return scy; }

u8 Dot::getScx() const { return scx; }

u8 Dot::getLy() const { return ly; }

u8 Dot::getLyc() const { return lyc; }

u8 Dot::getLx() const { return lx; }

u8 Dot::getWy() const { return wy; }

u8 Dot::getWx() const { return wx; }

void Dot::setScy(u8 value) { scy = value; }

void Dot::setScx(u8 value) { scx = value; }

void Dot::setLy(u8 value) { ly = value; }

void Dot::setLyc(u8 value) { lyc = value; }

void Dot::setWy(u8 value) { wy = value; }

void Dot::setWx(u8 value) { wx = value; }

}  // namespace gbemu
