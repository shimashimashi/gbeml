#include "core/graphics/ppu.h"

#include "glog/logging.h"

namespace gbeml {

void Ppu::tick() {
  if (!lcdc.isLcdEnabled()) {
    return;
  }

  switch (mode) {
    case PpuMode::Drawing:
      draw();
      break;
    case PpuMode::OamScan:
    case PpuMode::HBlank:
    case PpuMode::VBlank:
      break;
      break;
  }

  moveNext();
}

void Ppu::moveNext() {
  if (++cycles % 456 == 0) {
    if (++ly == 154) {
      ly = 0;
    }
  }

  cycles %= 456 * 154;

  switch (mode) {
    case PpuMode::OamScan:
      if (cycles % 456 == 80) {
        enterDrawing();
      }
      break;
    case PpuMode::Drawing:
      if (shifter_x == 160) {
        enterHBlank();
      }
      break;
    case PpuMode::HBlank:
      if (cycles % 456 == 0) {
        if (ly == 144) {
          enterVBlank();
        } else {
          enterOamScan();
        }
      }
      break;
    case PpuMode::VBlank:
      if (ly == 0) {
        enterOamScan();
      }
      break;
  }
}

void Ppu::draw() {
  if (stalls > 0) {
    stalls--;
    return;
  }

  if (reachWindow()) {
    enterWindow();
    return;
  }

  initiateSpriteFetch();
  if (visible_sprites.size() > 0) {
    fetchSpritePixels();
    stalls += 10;
    return;
  }

  if (fetcher_stalls > 0) {
    fetcher_stalls--;
  } else {
    if (draw_window) {
      fetchWindowPixels();
    } else {
      fetchBackgroundPixels();
    }
    fetcher_stalls += 7;
  }

  shiftPixel();
}

void Ppu::fetchBackgroundPixels() {
  Tile tile = pixel_fetcher.fetchBackgroundPixels(scx, scy, ly);

  for (u8 i = 0; i < 8; ++i) {
    Color color = tile.getAt(i);
    background_fifo.push(color);
  }

  while (num_unused_pixels > 0) {
    background_fifo.pop();
    num_unused_pixels--;
  }
}

void Ppu::fetchWindowPixels() {
  Tile tile = pixel_fetcher.fetchWindowPixels(window_line_counter - 1);

  for (u8 i = 0; i < 8; ++i) {
    Color color = tile.getAt(i);
    background_fifo.push(color);
  }

  while (num_unused_pixels > 0) {
    background_fifo.pop();
    num_unused_pixels--;
  }
}

void Ppu::fetchSpritePixels() {
  std::vector<SpritePixel> pixels =
      pixel_fetcher.fetchSpritePixels(shifter_x, ly, visible_sprites);
  visible_sprites.clear();

  u64 num_skip = sprite_fifo.size();
  for (const SpritePixel& pixel : pixels) {
    if (num_skip > 0) {
      num_skip--;
      continue;
    }
    sprite_fifo.push(pixel);
  }
}

void Ppu::shiftPixel() {
  if (background_fifo.size() == 0) {
    return;
  }

  BackgroundPixel background_pixel = background_fifo.front();
  background_fifo.pop();

  Color color = background_pixel.getColor();
  if (!lcdc.isBackgroundEnabled()) {
    color = Color::White;
  }

  if (sprite_fifo.size() > 0) {
    SpritePixel sprite_pixel = sprite_fifo.front();
    sprite_fifo.pop();

    if (sprite_pixel.getColor() != Color::Transparent &&
        (!sprite_pixel.isBackgroundOverSprite() || color == Color::White) &&
        lcdc.isSpriteEnabled()) {
      color = sprite_pixel.getColor();
    }
  }

  display->render(shifter_x++, ly, color);
}

void Ppu::scanOam() {
  for (u64 i = 0; i < 160; i += 4) {
    if (sprite_buffer.size() >= 10) {
      continue;
    }

    u8 y = oam->read(i);
    u8 x = oam->read(i + 1);
    u8 tile_index = oam->read(i + 2);
    u8 flags = oam->read(i + 3);

    Sprite sprite(y, x, tile_index, flags);
    if (sprite.isVisibleVertically(ly, lcdc.spriteSize())) {
      sprite_buffer.push_back(sprite);
    }
  }
}

void Ppu::initiateSpriteFetch() {
  for (Sprite& sprite : sprite_buffer) {
    if (sprite.isDrawn()) {
      continue;
    }

    if (sprite.isVisibleHorizontally(shifter_x)) {
      sprite.setDrawn();
      visible_sprites.push_back(sprite);
    }
  }
}

bool Ppu::reachWindow() {
  if (draw_window) {
    return false;
  }

  return lcdc.isWindowEnabled() && is_window_visible_vertically &&
         shifter_x >= wx - 7;
}

void Ppu::init() {
  if (ly >= 144) {
    mode = PpuMode::VBlank;
    enterVBlank();
  } else {
    mode = PpuMode::OamScan;
    enterOamScan();
  }
}

void Ppu::enterOamScan() {
  mode = PpuMode::OamScan;
  if (lcd_stat.isOamScanInterruptEnabled()) {
    ic->signalLcdStat();
  }
  if (lyc == ly && lcd_stat.isLycEqualsLyInterruptEnabled()) {
    ic->signalLcdStat();
  }
  if (ly >= wy) {
    is_window_visible_vertically = true;
  }

  scanOam();
}

void Ppu::enterDrawing() {
  mode = PpuMode::Drawing;
  stalls = 12;
  shifter_x = 0;
  pixel_fetcher.reset();
  fetchBackgroundPixels();
  num_unused_pixels = scx % 8;
}

void Ppu::enterHBlank() {
  mode = PpuMode::HBlank;
  while (background_fifo.size() > 0) {
    background_fifo.pop();
  }
  while (sprite_fifo.size() > 0) {
    sprite_fifo.pop();
  }
  sprite_buffer.clear();
  draw_window = false;
}

void Ppu::enterVBlank() {
  mode = PpuMode::VBlank;
  ic->signalVBlank();
  if (lcd_stat.isVBlankInterruptEnabled()) {
    ic->signalLcdStat();
  }
  if (lyc == ly && lcd_stat.isLycEqualsLyInterruptEnabled()) {
    ic->signalLcdStat();
  }
  is_window_visible_vertically = false;
  window_line_counter = 0;
}

void Ppu::enterWindow() {
  draw_window = true;
  while (background_fifo.size() > 0) {
    background_fifo.pop();
  }
  pixel_fetcher.reset();
  if (wx < 7) {
    num_unused_pixels = 7 - wx;
  }
  window_line_counter++;
  fetchWindowPixels();
  stalls += 5;
}

u8 Ppu::readVram(u16 addr) const {
  if (!lcdc.isLcdEnabled()) {
    return vram->read(addr);
  }

  switch (mode) {
    case PpuMode::Drawing:
      return 0xff;
    case PpuMode::OamScan:
    case PpuMode::HBlank:
    case PpuMode::VBlank:
      return vram->read(addr);
  }
}

u8 Ppu::readOam(u16 addr) const {
  if (!lcdc.isLcdEnabled()) {
    return oam->read(addr);
  }

  switch (mode) {
    case PpuMode::OamScan:
    case PpuMode::Drawing:
      return 0xff;
    case PpuMode::HBlank:
    case PpuMode::VBlank:
      return oam->read(addr);
  }
}

u8 Ppu::readLcdc() const { return lcdc.read(); }

u8 Ppu::readLcdStat() const {
  // lower 3 bits are garanteed to be cleared.
  u8 n = lcd_stat.read();

  if (lyc == ly) {
    n |= 0b100;
  }

  switch (mode) {
    case PpuMode::HBlank:
      return n | 0b00;
    case PpuMode::VBlank:
      return n | 0b01;
    case PpuMode::OamScan:
      return n | 0b10;
    case PpuMode::Drawing:
      return n | 0b11;
  }
}

u8 Ppu::readScy() const { return scy; }

u8 Ppu::readScx() const { return scx; }

u8 Ppu::readLy() const { return ly; }

u8 Ppu::readLyc() const { return lyc; }

u8 Ppu::readWy() const { return wy; }

u8 Ppu::readWx() const { return wx; }

u8 Ppu::readBgp() const { return bgp.read(); }

u8 Ppu::readObp0() const { return obp0.read(); }

u8 Ppu::readObp1() const { return obp1.read(); }

void Ppu::writeVram(u16 addr, u8 value) {
  if (!lcdc.isLcdEnabled()) {
    vram->write(addr, value);
    return;
  }

  switch (mode) {
    case PpuMode::Drawing:
      return;
    case PpuMode::OamScan:
    case PpuMode::HBlank:
    case PpuMode::VBlank:
      vram->write(addr, value);
      return;
  }
}

void Ppu::writeOam(u16 addr, u8 value) {
  if (!lcdc.isLcdEnabled()) {
    oam->write(addr, value);
    return;
  }

  switch (mode) {
    case PpuMode::OamScan:
    case PpuMode::Drawing:
      return;
    case PpuMode::HBlank:
    case PpuMode::VBlank:
      oam->write(addr, value);
      return;
  }
}

void Ppu::writeLcdc(u8 value) { lcdc.write(value); }

void Ppu::writeLcdStat(u8 value) { lcd_stat.write(value); }

void Ppu::writeScy(u8 value) { scy = value; }

void Ppu::writeScx(u8 value) { scx = value; }

void Ppu::writeLy(u8 value) { ly = value; }

void Ppu::writeLyc(u8 value) { lyc = value; }

void Ppu::writeWy(u8 value) { wy = value; }

void Ppu::writeWx(u8 value) { wx = value; }

void Ppu::writeBgp(u8 value) { bgp.write(value); }

void Ppu::writeObp0(u8 value) { obp0.write(value); }

void Ppu::writeObp1(u8 value) { obp1.write(value); }

PpuMode Ppu::getMode() { return mode; }

}  // namespace gbeml
