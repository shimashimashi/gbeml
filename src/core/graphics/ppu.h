#ifndef GBEML_PPU_H_
#define GBEML_PPU_H_

#include <queue>
#include <vector>

#include "core/display/display.h"
#include "core/graphics/color.h"
#include "core/graphics/fetcher.h"
#include "core/graphics/lcd_stat.h"
#include "core/graphics/lcdc.h"
#include "core/graphics/mode.h"
#include "core/graphics/palette.h"
#include "core/graphics/pixel.h"
#include "core/graphics/sprite.h"
#include "core/graphics/tile.h"
#include "core/interrupt/interrupt_controller.h"
#include "core/memory/ram.h"
#include "core/types/types.h"

namespace gbeml {

class Ppu {
 public:
  Ppu(Display* display_, Ram* vram_, Ram* oam_, InterruptController* ic_)
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

  u64 stalls = 0;
  u64 fetcher_stalls = 0;
  u8 num_unused_pixels = 0;
  u64 cycles = 0;
  bool draw_window = false;
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
  void enterDrawing();
  void enterHBlank();
  void enterVBlank();
  void enterWindow();
};

}  // namespace gbeml

#endif  // GBEML_PPU_H_
