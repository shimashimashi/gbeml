#include "core/graphics/ppu.h"

#include <array>
#include <cassert>
#include <cstdio>

#include "glog/logging.h"

namespace gbeml {

u8 Lcdc::read() const { return flags.get(); }

void Lcdc::write(u8 value) { return flags.set(value); }

bool Lcdc::isLcdEnabled() const { return flags.getAt(7); }

u16 Lcdc::getWindowTileMapAddress(u16 offset) const {
  u16 base = flags.getAt(6) ? 0x1c00 : 0x1800;
  return base + offset;
}

bool Lcdc::isWindowEnabled() const { return flags.getAt(5); }

u16 Lcdc::getBackgroundTileDataAddress(u8 tile_number) const {
  if (flags.getAt(4)) {
    return tile_number * 16;
  } else {
    return static_cast<u16>(0x1000 + static_cast<i8>(tile_number) * 16);
  }
}

u16 Lcdc::getBackgroundTileMapAddress(u16 offset) const {
  u16 base = flags.getAt(3) ? 0x1c00 : 0x1800;
  return base + offset;
}

SpriteSize Lcdc::spriteSize() const {
  return flags.getAt(2) ? SpriteSize::Tall : SpriteSize::Short;
}

bool Lcdc::isSpriteEnabled() const { return flags.getAt(1); }

bool Lcdc::isBackgroundEnabled() const { return flags.getAt(0); }

u8 LcdStat::read() const {
  // clear lower 3 bits to be set later.
  return flags.get() & 0b11111000;
}

void LcdStat::write(u8 value) { return flags.set(value); }

bool LcdStat::isLycEqualsLyInterruptEnabled() const { return flags.getAt(6); }

bool LcdStat::isOamScanInterruptEnabled() const { return flags.getAt(5); }

bool LcdStat::isVBlankInterruptEnabled() const { return flags.getAt(4); }

bool LcdStat::isHBlankInterruptEnabled() const { return flags.getAt(3); }

void Ppu::tick() {
  if (!lcdc.isLcdEnabled()) {
    return;
  }

  switch (mode) {
    case PpuMode::OamScan:
      break;
    case PpuMode::Drawing:
      draw();
      break;
    case PpuMode::HBlank:
      break;
    case PpuMode::VBlank:
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

u8 Ppu::getBackgroundTileNumber() {
  u8 x = ((scx + fetcher_x) & 0xff) / 8;
  u8 y = ((scy + ly) & 0xff) / 8;
  u16 offset = (x + y * 32) & 0x3ff;
  u16 addr = lcdc.getBackgroundTileMapAddress(offset);
  return vram->read(addr);
}

u16 Ppu::getBackgroundTileDataAddress(u8 tile_number) {
  u16 base = lcdc.getBackgroundTileDataAddress(tile_number);
  u16 offset = 2 * ((ly + scy) % 8);
  return base + offset;
}

u8 Ppu::getWindowTileNumber() {
  u8 x = (fetcher_x & 0xff) / 8;
  u8 y = ((window_line_counter - 1) & 0xff) / 8;
  u16 offset = (x + y * 32) & 0x3ff;
  u16 addr = lcdc.getWindowTileMapAddress(offset);
  return vram->read(addr);
}

u16 Ppu::getWindowTileDataAddress(u8 tile_number) {
  u16 base = lcdc.getBackgroundTileDataAddress(tile_number);
  u16 offset = 2 * ((window_line_counter - 1) % 8);
  return base + offset;
}

void Ppu::fetchBackgroundPixels() {
  u8 tile_number = getBackgroundTileNumber();
  u16 addr = getBackgroundTileDataAddress(tile_number);

  u8 low = vram->read(addr);
  u8 high = vram->read(addr + 1);

  Tile tile(low, high, bgp);
  for (u8 i = 0; i < 8; ++i) {
    Color color = tile.getAt(i);
    background_fifo.push(color);
  }

  fetcher_x += 8;
}

void Ppu::fetchWindowPixels() {
  u8 tile_number = getWindowTileNumber();
  u16 addr = getWindowTileDataAddress(tile_number);

  u8 low = vram->read(addr);
  u8 high = vram->read(addr + 1);

  Tile tile(low, high, bgp);
  for (u8 i = 0; i < 8; ++i) {
    Color color = tile.getAt(i);
    background_fifo.push(color);
  }

  fetcher_x += 8;
}

void Ppu::fetchSpritePixels() {
  std::vector<SpritePixel> pixels(8, SpritePixel(Color::Transparent, true));

  for (const Sprite& sprite : visible_sprites) {
    u16 addr = sprite.getTileDataAddress(ly, lcdc.spriteSize());
    u8 low = vram->read(addr);
    u8 high = vram->read(addr + 1);
    SpritePalette& palette = sprite.getPaletteNumber() ? obp1 : obp0;

    Tile tile(low, high, palette);
    if (sprite.flipX()) {
      tile.reverse();
    }

    for (u8 i = 0; i < 8; ++i) {
      if (pixels[i].getColor() != Color::Transparent) {
        continue;
      }
      if (shifter_x + i >= sprite.getX()) {
        continue;
      }
      Color color = tile.getAt(shifter_x + i - sprite.getX() + 8);
      if (color != Color::Transparent) {
        pixels[i] = SpritePixel(color, sprite.isBackgroundOverSprite());
      }
    }
  }

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

  if (num_unused_pixels > 0) {
    num_unused_pixels--;
    return;
  }

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
  fetcher_x = 0;
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
  fetcher_x = 0;
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
