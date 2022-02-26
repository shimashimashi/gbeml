#include "graphics/ppu.h"

#include <array>
#include <cassert>
#include <cstdio>

namespace gbemu {

u8 Lcdc::read() const { return flags.get(); }

void Lcdc::write(u8 value) { return flags.set(value); }

bool Lcdc::isLcdEnabled() const { return flags.getAt(7); }

u16 Lcdc::windowTileMapArea() const { return flags.getAt(6) ? 0x1800 : 0x1c00; }

bool Lcdc::isWindowEnabled() const { return flags.getAt(5); }

u16 Lcdc::bgTileDataAddress(u8 offset) const {
  if (flags.getAt(4)) {
    return offset * 16;
  } else {
    return static_cast<u16>(0x1000 + static_cast<i8>(offset) * 16);
  }
}

u16 Lcdc::bgTileMapAddress(u16 offset) const {
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

u8 Palette::read() const { return data.get(); }

void Palette::write(u8 value) { return data.set(value); }

Color Palette::getColor(u8 color_id) {
  u8 color = 0;
  switch (color_id) {
    case 0:
      color = data.slice(0, 1);
      break;
    case 1:
      color = data.slice(2, 3);
      break;
    case 2:
      color = data.slice(4, 5);
      break;
    case 3:
      color = data.slice(6, 7);
      break;
    default:
      assert(false);
      break;
  }

  switch (color) {
    case 0:
      return Color::White;
    case 1:
      return Color::LightGray;
    case 2:
      return Color::DarkGray;
    case 3:
      return Color::Black;
    default:
      assert(false);
      return Color::White;
  }
}

void Ppu::tick() {
  if (!lcdc.isLcdEnabled()) {
    return;
  }

  if (dot.requestsVBlankInterrupt() && lcdStat.isVBlankInterruptEnabled()) {
    ic->signalVBlank();
  }

  if (requestsLcdStat()) {
    ic->signalLcdStat();
  }

  switch (dot.getMode()) {
    case PpuMode::OamScan:
      break;
    case PpuMode::Drawing:
      if (dot.fetch()) {
        fetchPixels();
      }
      break;
    case PpuMode::HBlank:
      if (!background_fifo.empty()) {
        pushPixels();
      }
      break;
    case PpuMode::VBlank:
      break;
  }

  dot.move();
}

void Ppu::fetchPixels() {
  u8 low = fetchLowTileData();
  u8 high = fetchHighTileData();

  for (i64 i = 7; i >= 0; --i) {
    if (dot.discardsCurrentPixel()) {
      continue;
    }

    u8 h = (high >> i) & 1;
    h <<= 1;
    u8 l = (low >> i) & 1;
    u8 color_id = h + l;
    Color color = bgp.getColor(color_id);

    if (background_fifo.size() < 160) {
      background_fifo.push(color);
    }
  }
}

void Ppu::pushPixels() {
  std::array<Color, 160> pixels;
  for (u8 i = 0; i < 160; ++i) {
    pixels[i] = background_fifo.front();
    background_fifo.pop();
  }

  display->pushRow(dot.getLy(), pixels);
}

u8 Ppu::readVram(u16 addr) const {
  if (!lcdc.isLcdEnabled()) {
    return vram->read(addr);
  }

  switch (dot.getMode()) {
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

  switch (dot.getMode()) {
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

  if (dot.isLycEqualsLy()) {
    n |= 0b100;
  }

  switch (dot.getMode()) {
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

u8 Ppu::readScy() const { return dot.getScy(); }

u8 Ppu::readScx() const { return dot.getScx(); }

u8 Ppu::readLy() const { return dot.getLy(); }

u8 Ppu::readLyc() const { return dot.getLyc(); }

u8 Ppu::readWy() const { return dot.getWy(); }

u8 Ppu::readWx() const { return dot.getWx(); }

u8 Ppu::readBgp() const { return bgp.read(); }

u8 Ppu::readObp0() const { return obp0.read(); }

u8 Ppu::readObp1() const { return obp1.read(); }

void Ppu::writeVram(u16 addr, u8 value) {
  if (!lcdc.isLcdEnabled()) {
    vram->write(addr, value);
    return;
  }

  switch (dot.getMode()) {
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

  switch (dot.getMode()) {
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

void Ppu::writeScy(u8 value) { dot.setScy(value); }

void Ppu::writeScx(u8 value) { dot.setScx(value); }

void Ppu::writeLy(u8 value) { dot.setLy(value); }

void Ppu::writeLyc(u8 value) { dot.setLyc(value); }

void Ppu::writeWy(u8 value) { dot.setWy(value); }

void Ppu::writeWx(u8 value) { dot.setWx(value); }

void Ppu::writeBgp(u8 value) { bgp.write(value); }

void Ppu::writeObp0(u8 value) { obp0.write(value); }

void Ppu::writeObp1(u8 value) { obp1.write(value); }

u8 Ppu::fetchTileNumber() {
  u16 offset = dot.getTileMapAddressOffset();
  u16 addr = lcdc.bgTileMapAddress(offset);
  return vram->read(addr);
}

u8 Ppu::fetchLowTileData() {
  u8 tile_number = fetchTileNumber();
  u16 base = lcdc.bgTileDataAddress(tile_number);
  u8 offset = dot.getLowTileDataAddressOffset();
  return vram->read(base + offset);
}

u8 Ppu::fetchHighTileData() {
  u8 tile_number = fetchTileNumber();
  u16 base = lcdc.bgTileDataAddress(tile_number);
  u8 offset = dot.getHighTileDataAddressOffset();
  return vram->read(base + offset);
}

bool Ppu::requestsLcdStat() {
  return (dot.requestsHBlankInterrupt() &&
          lcdStat.isHBlankInterruptEnabled()) ||
         (dot.requestsOamScanInterrupt() &&
          lcdStat.isOamScanInterruptEnabled()) ||
         (dot.requestsLycEqualsLyInterrupt() &&
          lcdStat.isLycEqualsLyInterruptEnabled()) ||
         (dot.requestsVBlankInterrupt() && lcdStat.isVBlankInterruptEnabled());
}

}  // namespace gbemu
