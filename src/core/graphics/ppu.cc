#include "core/graphics/ppu.h"

#include <array>
#include <cassert>
#include <cstdio>

#include "glog/logging.h"

namespace gbemu {

u8 Lcdc::read() const { return flags.get(); }

void Lcdc::write(u8 value) { return flags.set(value); }

bool Lcdc::isLcdEnabled() const { return flags.getAt(7); }

u16 Lcdc::windowTileMapArea() const { return flags.getAt(6) ? 0x1800 : 0x1c00; }

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

ObjSize Lcdc::objSize() const {
  return flags.getAt(2) ? ObjSize::Tall : ObjSize::Normal;
}

bool Lcdc::isObjEnabled() const { return flags.getAt(1); }

bool Lcdc::isBgEnabled() const { return flags.getAt(0); }

u8 LcdStat::read() const {
  // clear lower 3 bits to be set later.
  u8 n = flags.slice(3, 6);
  return n << 3;
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

  if (fetcher_stalls > 0) {
    fetcher_stalls--;
  } else {
    fetchPixels();
    fetcher_stalls += 7;
  }

  if (background_fifo.size() > 0) {
    shiftPixel();
  }
}

u8 Ppu::getBackgroundTileNumber() {
  u8 x = ((scx + fetcher_x) & 0xff) / 8;
  u8 y = ((scy + ly) & 0xff) / 8;
  u16 offset = (x + y * 32) & 0x3ff;
  u16 addr = lcdc.getBackgroundTileMapAddress(offset);
  return vram->read(addr);
}

u8 Ppu::getBackgroundLowTileData(u8 tile_number) {
  u16 base = lcdc.getBackgroundTileDataAddress(tile_number);
  u16 offset = 2 * ((ly + scy) % 8);
  return vram->read(base + offset);
}

u8 Ppu::getBackgroundHighTileData(u8 tile_number) {
  u16 base = lcdc.getBackgroundTileDataAddress(tile_number);
  u16 offset = 2 * ((ly + scy) % 8) + 1;
  return vram->read(base + offset);
}

void Ppu::fetchPixels() {
  u8 tile_number = getBackgroundTileNumber();
  VLOG(1) << "tile_number: " << (int)tile_number << std::endl;

  u8 low = getBackgroundLowTileData(tile_number);
  u8 high = getBackgroundHighTileData(tile_number);

  background_fifo.pushTileData(low, high);

  fetcher_x += 8;
}

void Ppu::shiftPixel() {
  Color pixel = background_fifo.popPixel();

  if (num_unused_pixels > 0) {
    num_unused_pixels--;
    return;
  }

  display->render(shifter_x++, ly, pixel);
}

void Ppu::init() {
  if (ly >= 144) {
    mode = PpuMode::VBlank;
  } else {
    mode = PpuMode::OamScan;
  }
}

void Ppu::enterOamScan() {
  mode = PpuMode::OamScan;
  if (lcdStat.isOamScanInterruptEnabled()) {
    ic->signalLcdStat();
  }
  if (lyc == ly && lcdStat.isLycEqualsLyInterruptEnabled()) {
    ic->signalLcdStat();
  }
}

void Ppu::enterDrawing() {
  mode = PpuMode::Drawing;
  stalls = 12;
  shifter_x = 0;
  fetcher_x = 0;
  fetchPixels();
  num_unused_pixels = scx % 8;
}

void Ppu::enterHBlank() {
  mode = PpuMode::HBlank;
  while (background_fifo.size() > 0) {
    background_fifo.popPixel();
  }
}

void Ppu::enterVBlank() {
  mode = PpuMode::VBlank;
  ic->signalVBlank();
  if (lcdStat.isVBlankInterruptEnabled()) {
    ic->signalLcdStat();
  }
  if (lyc == ly && lcdStat.isLycEqualsLyInterruptEnabled()) {
    ic->signalLcdStat();
  }
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
  u8 n = lcdStat.read();

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

u8 Ppu::readBgp() const { return background_fifo.getPalette(); }

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

void Ppu::writeLcdStat(u8 value) { lcdStat.write(value); }

void Ppu::writeScy(u8 value) { scy = value; }

void Ppu::writeScx(u8 value) { scx = value; }

void Ppu::writeLy(u8 value) { ly = value; }

void Ppu::writeLyc(u8 value) { lyc = value; }

void Ppu::writeWy(u8 value) { wy = value; }

void Ppu::writeWx(u8 value) { wx = value; }

void Ppu::writeBgp(u8 value) { background_fifo.setPalette(value); }

void Ppu::writeObp0(u8 value) { obp0.write(value); }

void Ppu::writeObp1(u8 value) { obp1.write(value); }

PpuMode Ppu::getMode() { return mode; }

}  // namespace gbemu
