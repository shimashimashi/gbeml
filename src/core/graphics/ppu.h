#ifndef GBEMU_PPU_H_
#define GBEMU_PPU_H_

#include "core/display/display.h"
#include "core/graphics/color.h"
#include "core/graphics/mode.h"
#include "core/graphics/palette.h"
#include "core/graphics/pixel_fifo.h"
#include "core/interrupt/interrupt_controller.h"
#include "core/memory/ram.h"
#include "core/types/types.h"

namespace gbemu {

enum class ObjSize { Normal, Tall };

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
  u16 getBackgroundTileDataAddress(u8 tile_number) const;
  u16 getBackgroundTileMapAddress(u16 offset) const;
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

 private:
  Register flags;
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
        obp1(0),
        background_fifo() {}

  void tick();
  void init();

  u8 readVram(u16 addr) const;
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

  void writeVram(u16 addr, u8 value);
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
  PixelFifo background_fifo;
  PpuMode mode;

  u8 scy = 0;
  u8 scx = 0;
  u8 ly = 0;
  u8 lyc = 0;
  u16 lx = 0;
  u8 wy = 0;
  u8 wx = 0;
  u8 x = 0;

  u64 stalls = 0;
  u64 fetch_stalls = 0;
  u8 num_unused_pixels = 0;

  void draw();
  void fetchPixels();
  void shiftPixel();
  void moveNext();

  void enterOamScan();
  void enterDrawing();
  void enterHBlank();
  void enterVBlank();

  u8 getBackgroundTileNumber();
  u8 getBackgroundLowTileData(u8 tile_number);
  u8 getBackgroundHighTileData(u8 tile_number);
};

}  // namespace gbemu

#endif  // GBEMU_PPU_H_