#ifndef GBEML_PPU_IMPL_H_
#define GBEML_PPU_IMPL_H_

#include <queue>
#include <vector>

#include "core/display/display.h"
#include "core/graphics/color.h"
#include "core/graphics/fetcher.h"
#include "core/graphics/lcd_stat.h"
#include "core/graphics/lcdc.h"
#include "core/graphics/palette.h"
#include "core/graphics/pixel.h"
#include "core/graphics/ppu.h"
#include "core/graphics/sprite.h"
#include "core/graphics/tile.h"
#include "core/interrupt/interrupt_controller.h"
#include "core/memory/ram.h"

namespace gbeml {

enum class PpuMode {
  HBlank,
  VBlank,
  OamScan,
  DrawingBackground,
  DrawingWindow
};

class PpuImpl : public Ppu {
 public:
  PpuImpl(Display* display_, Ram* vram_, Ram* oam_, InterruptController* ic_)
      : display(display_),
        vram(vram_),
        oam(oam_),
        ic(ic_),
        lcdc(0),
        lcd_stat(0),
        bgp(0),
        obp0(0),
        obp1(0),
        pixel_fetcher(*vram, lcdc, bgp, obp0, obp1) {}

  void tick() override;
  void init() override;

  u8 readVram(u16 addr) const override;
  u8 readOam(u16 addr) const override;

  u8 readLcdc() const override;
  u8 readLcdStat() const override;
  u8 readScy() const override;
  u8 readScx() const override;
  u8 readLy() const override;
  u8 readLyc() const override;
  u8 readWy() const override;
  u8 readWx() const override;
  u8 readBgp() const override;
  u8 readObp0() const override;
  u8 readObp1() const override;

  void writeVram(u16 addr, u8 value) override;
  void writeOam(u16 addr, u8 value) override;

  void writeLcdc(u8 value) override;
  void writeLcdStat(u8 value) override;
  void writeScy(u8 value) override;
  void writeScx(u8 value) override;
  void writeLy(u8 value) override;
  void writeLyc(u8 value) override;
  void writeWy(u8 value) override;
  void writeWx(u8 value) override;
  void writeBgp(u8 value) override;
  void writeObp0(u8 value) override;
  void writeObp1(u8 value) override;

  PpuMode getMode();

 private:
  Display* display;
  Ram* vram;
  Ram* oam;
  InterruptController* ic;

  Lcdc lcdc;
  LcdStat lcd_stat;
  BackgroundPalette bgp;
  SpritePalette obp0;
  SpritePalette obp1;
  PpuMode mode;
  PixelFetcher pixel_fetcher;

  std::queue<BackgroundPixel> background_fifo;
  std::queue<SpritePixel> sprite_fifo;
  std::vector<Sprite> sprite_buffer;
  std::vector<Sprite> visible_sprites;

  u8 scy = 0;
  u8 scx = 0;
  u8 ly = 0;
  u8 lyc = 0;
  u8 wy = 0;
  u8 wx = 0;

  u8 shifter_x = 0;
  // 1-index
  u8 window_line_counter = 0;
  u8 oam_counter = 0;

  u64 stalls = 0;
  u64 fetcher_stalls = 0;
  u8 num_unused_pixels = 0;
  u64 cycles = 0;
  bool is_window_visible_vertically = false;

  void draw();
  void fetchBackgroundPixels();
  void fetchWindowPixels();
  void fetchSpritePixels();
  void shiftPixel();

  void scanOam();
  void moveNext();
  void initiateSpriteFetch();
  bool reachWindow();

  void enterOamScan();
  void enterHBlank();
  void enterVBlank();
  void enterDrawingBackground();
  void enterDrawingWindow();
};

}  // namespace gbeml

#endif  // GBEML_PPU_IMPL_H_
