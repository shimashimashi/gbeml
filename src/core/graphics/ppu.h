#ifndef GBEML_PPU_H_
#define GBEML_PPU_H_

#include <queue>
#include <vector>

#include "core/types/types.h"

namespace gbeml {

class Ppu {
 public:
  virtual ~Ppu();

  virtual void tick() = 0;
  virtual void init() = 0;

  virtual u8 readVram(u16 addr) const = 0;
  virtual u8 readOam(u16 addr) const = 0;

  virtual u8 readLcdc() const = 0;
  virtual u8 readLcdStat() const = 0;
  virtual u8 readScy() const = 0;
  virtual u8 readScx() const = 0;
  virtual u8 readLy() const = 0;
  virtual u8 readLyc() const = 0;
  virtual u8 readWy() const = 0;
  virtual u8 readWx() const = 0;
  virtual u8 readBgp() const = 0;
  virtual u8 readObp0() const = 0;
  virtual u8 readObp1() const = 0;

  virtual void writeVram(u16 addr, u8 value) = 0;
  virtual void writeOam(u16 addr, u8 value) = 0;

  virtual void writeLcdc(u8 value) = 0;
  virtual void writeLcdStat(u8 value) = 0;
  virtual void writeScy(u8 value) = 0;
  virtual void writeScx(u8 value) = 0;
  virtual void writeLy(u8 value) = 0;
  virtual void writeLyc(u8 value) = 0;
  virtual void writeWy(u8 value) = 0;
  virtual void writeWx(u8 value) = 0;
  virtual void writeBgp(u8 value) = 0;
  virtual void writeObp0(u8 value) = 0;
  virtual void writeObp1(u8 value) = 0;
};

}  // namespace gbeml

#endif  // GBEML_PPU_H_
