#include "core/graphics/ppu_impl.h"

#include "core/log/logging.h"

namespace gbeml {

void PpuImpl::tick() {
  if (!lcdc.isLcdEnabled()) {
    return;
  }

  switch (mode) {
    case PpuMode::DrawingBackground:
    case PpuMode::DrawingWindow:
      draw();
      break;
    case PpuMode::OamScan:
      scanOam();
      break;
    case PpuMode::HBlank:
    case PpuMode::VBlank:
      break;
  }

  moveNext();
}

void PpuImpl::moveNext() {
  if (++cycles % 456 == 0) {
    if (++ly == 154) {
      ly = 0;
    }
  }

  cycles %= 456 * 154;

  switch (mode) {
    case PpuMode::OamScan:
      if (cycles % 456 == 80) {
        enterDrawingBackground();
      }
      break;
    case PpuMode::DrawingBackground:
      if (shifter_x == 160) {
        enterHBlank();
        break;
      }
      if (reachWindow()) {
        enterDrawingWindow();
      }
      break;
    case PpuMode::DrawingWindow:
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

void PpuImpl::draw() {
  if (stalls > 0) {
    stalls--;
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
    switch (mode) {
      case PpuMode::DrawingBackground:
        fetchBackgroundPixels();
        break;
      case PpuMode::DrawingWindow:
        fetchWindowPixels();
        break;
      default:
        DCHECK(false);
        break;
    }
    fetcher_stalls += 7;
  }

  shiftPixel();
}

void PpuImpl::fetchBackgroundPixels() {
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

void PpuImpl::fetchWindowPixels() {
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

void PpuImpl::fetchSpritePixels() {
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

void PpuImpl::shiftPixel() {
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

void PpuImpl::scanOam() {
  if (stalls > 0) {
    stalls--;
    return;
  }

  if (sprite_buffer.size() >= 10) {
    return;
  }

  u8 y = oam->read(oam_counter);
  u8 x = oam->read(oam_counter + 1);
  u8 tile_index = oam->read(oam_counter + 2);
  u8 flags = oam->read(oam_counter + 3);

  Sprite sprite(y, x, tile_index, flags);
  if (sprite.isVisibleVertically(ly, lcdc.spriteSize())) {
    sprite_buffer.push_back(sprite);
  }

  oam_counter += 4;
  stalls += 1;
}

void PpuImpl::initiateSpriteFetch() {
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

bool PpuImpl::reachWindow() {
  return lcdc.isWindowEnabled() && is_window_visible_vertically &&
         shifter_x >= wx - 7;
}

void PpuImpl::init() {
  if (ly >= 144) {
    mode = PpuMode::VBlank;
    enterVBlank();
  } else {
    mode = PpuMode::OamScan;
    enterOamScan();
  }
}

void PpuImpl::enterOamScan() {
  mode = PpuMode::OamScan;
  stalls = 0;
  if (lcd_stat.isOamScanInterruptEnabled()) {
    ic->signalLcdStat();
  }
  if (lyc == ly && lcd_stat.isLycEqualsLyInterruptEnabled()) {
    ic->signalLcdStat();
  }
  if (ly >= wy) {
    is_window_visible_vertically = true;
  }
  oam_counter = 0;
}

void PpuImpl::enterDrawingBackground() {
  mode = PpuMode::DrawingBackground;
  stalls = 12;
  shifter_x = 0;
  pixel_fetcher.reset();
  fetchBackgroundPixels();
  num_unused_pixels = scx % 8;
}

void PpuImpl::enterHBlank() {
  mode = PpuMode::HBlank;
  while (background_fifo.size() > 0) {
    background_fifo.pop();
  }
  while (sprite_fifo.size() > 0) {
    sprite_fifo.pop();
  }
  sprite_buffer.clear();
}

void PpuImpl::enterVBlank() {
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

void PpuImpl::enterDrawingWindow() {
  mode = PpuMode::DrawingWindow;
  while (background_fifo.size() > 0) {
    background_fifo.pop();
  }
  pixel_fetcher.reset();
  if (wx < 7) {
    num_unused_pixels = 7 - wx;
  } else {
    num_unused_pixels = 0;
  }
  window_line_counter++;
  fetchWindowPixels();
  stalls += 6;
}

u8 PpuImpl::readVram(u16 addr) const {
  if (!lcdc.isLcdEnabled()) {
    return vram->read(addr);
  }

  switch (mode) {
    case PpuMode::DrawingBackground:
    case PpuMode::DrawingWindow:
      return 0xff;
    case PpuMode::OamScan:
    case PpuMode::HBlank:
    case PpuMode::VBlank:
      return vram->read(addr);
  }
}

u8 PpuImpl::readOam(u16 addr) const {
  if (!lcdc.isLcdEnabled()) {
    return oam->read(addr);
  }

  switch (mode) {
    case PpuMode::OamScan:
    case PpuMode::DrawingBackground:
    case PpuMode::DrawingWindow:
      return 0xff;
    case PpuMode::HBlank:
    case PpuMode::VBlank:
      return oam->read(addr);
  }
}

u8 PpuImpl::readLcdc() const { return lcdc.read(); }

u8 PpuImpl::readLcdStat() const {
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
    case PpuMode::DrawingBackground:
    case PpuMode::DrawingWindow:
      return n | 0b11;
  }
}

u8 PpuImpl::readScy() const { return scy; }

u8 PpuImpl::readScx() const { return scx; }

u8 PpuImpl::readLy() const { return ly; }

u8 PpuImpl::readLyc() const { return lyc; }

u8 PpuImpl::readWy() const { return wy; }

u8 PpuImpl::readWx() const { return wx; }

u8 PpuImpl::readBgp() const { return bgp.read(); }

u8 PpuImpl::readObp0() const { return obp0.read(); }

u8 PpuImpl::readObp1() const { return obp1.read(); }

void PpuImpl::writeVram(u16 addr, u8 value) {
  if (!lcdc.isLcdEnabled()) {
    vram->write(addr, value);
    return;
  }

  switch (mode) {
    case PpuMode::DrawingBackground:
    case PpuMode::DrawingWindow:
      return;
    case PpuMode::OamScan:
    case PpuMode::HBlank:
    case PpuMode::VBlank:
      vram->write(addr, value);
      return;
  }
}

void PpuImpl::writeOam(u16 addr, u8 value) {
  if (!lcdc.isLcdEnabled()) {
    oam->write(addr, value);
    return;
  }

  switch (mode) {
    case PpuMode::OamScan:
    case PpuMode::DrawingBackground:
    case PpuMode::DrawingWindow:
      return;
    case PpuMode::HBlank:
    case PpuMode::VBlank:
      oam->write(addr, value);
      return;
  }
}

void PpuImpl::writeLcdc(u8 value) { lcdc.write(value); }

void PpuImpl::writeLcdStat(u8 value) { lcd_stat.write(value); }

void PpuImpl::writeScy(u8 value) { scy = value; }

void PpuImpl::writeScx(u8 value) { scx = value; }

void PpuImpl::writeLy(u8 value) { ly = value; }

void PpuImpl::writeLyc(u8 value) { lyc = value; }

void PpuImpl::writeWy(u8 value) { wy = value; }

void PpuImpl::writeWx(u8 value) { wx = value; }

void PpuImpl::writeBgp(u8 value) { bgp.write(value); }

void PpuImpl::writeObp0(u8 value) { obp0.write(value); }

void PpuImpl::writeObp1(u8 value) { obp1.write(value); }

PpuMode PpuImpl::getMode() { return mode; }

}  // namespace gbeml
