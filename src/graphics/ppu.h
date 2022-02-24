#ifndef GBEMU_PPU_H_
#define GBEMU_PPU_H_

#include <queue>

#include "graphics/color.h"
#include "graphics/display.h"
#include "interrupt/interrupt_controller.h"
#include "memory/ram.h"
#include "types/types.h"

namespace gbemu {

enum class ObjSize { Normal, Tall };

enum class PpuMode { HBlank, VBlank, OamScan, Drawing };

class Lcdc {
 public:
  Lcdc(u8 flags_) : flags(flags_) {}

  u8 read() const;
  void write(u8 value);

  bool isLcdEnabled() const;
  bool isWindowEnabled() const;
  bool isObjEnabled() const;
  bool isBgEnabled() const;

  u16 windowTileMapArea() const;
  u16 bgTileDataArea() const;
  u16 bgTileMapArea() const;
  ObjSize objSize() const;

 private:
  Register flags;
};

class LcdStat {
 public:
  LcdStat(u8 flags_) : flags(flags_) {}

  u8 read() const;
  void write(u8 value);

  bool isLycEqualsLyInterruptEnabled() const;
  bool isOamScanInterruptEnabled() const;
  bool isVBlankInterruptEnabled() const;
  bool isHBlankInterruptEnabled() const;
  bool isLycEqualsLy() const;

  PpuMode getMode() const;
  void setMode(PpuMode mode);

 private:
  Register flags;
};

class Palette {
 public:
  Palette(u8 data_) : data(data_) {}

  u8 read() const;
  void write(u8 value);
  Color getColor(u8 color_id);

 private:
  Register data;
};

class Ppu {
 public:
  Ppu(Display* display_, Ram* vram_, Ram* oam_, InterruptController* ic_)
      : display(display_),
        vram(vram_),
        oam(oam_),
        ic(ic_),
        lcdc(0),
        lcdStat(0),
        bgp(0),
        obp0(0),
        obp1(0) {}

  void tick();

  u8 readTileData(u16 addr) const;
  u8 readTileMap(u16 addr) const;
  u8 readOam(u16 addr) const;

  u8 readLcdc() const;
  u8 readLcdStat() const;
  u8 readScy() const;
  u8 readScx() const;
  u8 readLy() const;
  u8 readLyc() const;
  u8 readWy() const;
  u8 readWx() const;
  u8 readBgp() const;
  u8 readObp0() const;
  u8 readObp1() const;

  void writeTileData(u16 addr, u8 value);
  void writeTileMap(u16 addr, u8 value);
  void writeOam(u16 addr, u8 value);

  void writeLcdc(u8 value);
  void writeLcdStat(u8 value);
  void writeScy(u8 value);
  void writeScx(u8 value);
  void writeLy(u8 value);
  void writeLyc(u8 value);
  void writeWy(u8 value);
  void writeWx(u8 value);
  void writeBgp(u8 value);
  void writeObp0(u8 value);
  void writeObp1(u8 value);

 private:
  Display* display;
  Ram* vram;
  Ram* oam;
  InterruptController* ic;

  Lcdc lcdc;
  LcdStat lcdStat;
  Palette bgp;
  Palette obp0;
  Palette obp1;

  u8 scy = 0;
  u8 scx = 0;
  u8 ly = 0;
  u8 lyc = 0;
  u8 lx = 0;
  u8 wy = 0;
  u8 wx = 0;
  u32 cycles = 0;

  std::queue<Color> background_fifo;

  void draw();

  u16 fetchTileNumber();
  u8 fetchLowTileData();
  u8 fetchHighTileData();
};

}  // namespace gbemu

#endif  // GBEMU_PPU_H_
