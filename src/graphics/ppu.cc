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

u8 LcdStat::read() const { return flags.get(); }

void LcdStat::write(u8 value) { return flags.set(value); }

bool LcdStat::isLycEqualsLyInterruptEnabled() const { return flags.getAt(6); }

bool LcdStat::isOamScanInterruptEnabled() const { return flags.getAt(5); }

bool LcdStat::isVBlankInterruptEnabled() const { return flags.getAt(4); }

bool LcdStat::isHBlankInterruptEnabled() const { return flags.getAt(3); }

bool LcdStat::isLycEqualsLy() const { return flags.getAt(2); }

void LcdStat::setLycEqualsLy(bool flag) { flags.setAt(2, flag); }

PpuMode LcdStat::getMode() const {
  u8 mode = flags.slice(0, 1);
  switch (mode) {
    case 0:
      return PpuMode::HBlank;
    case 1:
      return PpuMode::VBlank;
    case 2:
      return PpuMode::OamScan;
    case 3:
      return PpuMode::Drawing;
    default:
      assert(false);
      return PpuMode::HBlank;
  }
}

void LcdStat::setMode(PpuMode mode) {
  switch (mode) {
    case PpuMode::HBlank:
      flags.setAt(1, 0);
      flags.setAt(0, 0);
      break;
    case PpuMode::VBlank:
      flags.setAt(1, 0);
      flags.setAt(0, 1);
      break;
    case PpuMode::OamScan:
      flags.setAt(1, 1);
      flags.setAt(0, 0);
      break;
    case PpuMode::Drawing:
      flags.setAt(1, 1);
      flags.setAt(0, 1);
      break;
  }
}

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
  lcdStat.setLycEqualsLy(ly == lyc);
  if (lcdStat.isLycEqualsLy() && lcdStat.isLycEqualsLyInterruptEnabled()) {
    ic->signalLcdStat();
  }

  switch (lcdStat.getMode()) {
    case PpuMode::VBlank:
      if (ly == 0) {
        lcdStat.setMode(PpuMode::OamScan);
        if (lcdStat.isOamScanInterruptEnabled()) {
          ic->signalLcdStat();
        }
      }
      break;
    case PpuMode::OamScan:
      if (lx == 80) {
        lcdStat.setMode(PpuMode::Drawing);
        while (!background_fifo.empty()) {
          background_fifo.pop();
        }
      }
      break;
    case PpuMode::Drawing:
      if (lx == 252) {
        lcdStat.setMode(PpuMode::HBlank);
        if (lcdStat.isHBlankInterruptEnabled()) {
          ic->signalLcdStat();
        }
      }
      break;
    case PpuMode::HBlank:
      if (lx == 0) {
        if (ly < 144) {
          lcdStat.setMode(PpuMode::OamScan);
          if (lcdStat.isOamScanInterruptEnabled()) {
            ic->signalLcdStat();
          }
        } else {
          lcdStat.setMode(PpuMode::VBlank);
          if (lcdStat.isVBlankInterruptEnabled()) {
            ic->signalLcdStat();
          }
        }
      }
      break;
  }

  switch (lcdStat.getMode()) {
    case PpuMode::OamScan:
      break;
    case PpuMode::Drawing:
      if (lx % 8 == 0) {
        draw();
      }
      break;
    case PpuMode::HBlank:
      break;
    case PpuMode::VBlank:
      break;
  }

  if (lx == 455) {
    ly = (ly + 1) % 154;
  }
  lx = (lx + 1) % 456;
}

void Ppu::draw() {
  u8 low = fetchLowTileData();
  u8 high = fetchHighTileData();

  u8 num_discards = scx % 8;

  for (i64 i = 7; i >= 0; --i) {
    if (lx == 80 && num_discards > 0) {
      num_discards--;
      continue;
    }
    u8 h = (high >> i) & 1;
    h <<= 1;
    u8 l = (low >> i) & 1;
    u8 color_id = h + l;
    Color color = bgp.getColor(color_id);
    background_fifo.push(color);
  }

  if (background_fifo.size() == 160) {
    std::array<Color, 160> pixels;
    for (u8 i = 0; i < 160; ++i) {
      pixels[i] = background_fifo.front();
      background_fifo.pop();
    }

    display->pushRow(ly, pixels);
  }
}

u8 Ppu::readVram(u16 addr) const {
  switch (lcdStat.getMode()) {
    case PpuMode::Drawing:
      return 0xff;
    case PpuMode::OamScan:
    case PpuMode::HBlank:
    case PpuMode::VBlank:
      return vram->read(addr);
  }
}

u8 Ppu::readOam(u16 addr) const {
  switch (lcdStat.getMode()) {
    case PpuMode::OamScan:
    case PpuMode::Drawing:
      return 0xff;
    case PpuMode::HBlank:
    case PpuMode::VBlank:
      return oam->read(addr);
  }
}

u8 Ppu::readLcdc() const { return lcdc.read(); }

u8 Ppu::readLcdStat() const { return lcdStat.read(); }

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
  // If we disable write while drawing, hello-world ROM doesn't run correctly.
  // TODO: fix this
  // switch (lcdStat.getMode()) {
  //   case PpuMode::Drawing:
  //     return;
  //   case PpuMode::OamScan:
  //   case PpuMode::HBlank:
  //   case PpuMode::VBlank:
  //     vram->write(addr, value);
  //     return;
  // }

  vram->write(addr, value);
}

void Ppu::writeOam(u16 addr, u8 value) {
  switch (lcdStat.getMode()) {
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

void Ppu::writeBgp(u8 value) { bgp.write(value); }

void Ppu::writeObp0(u8 value) { obp0.write(value); }

void Ppu::writeObp1(u8 value) { obp1.write(value); }

u8 Ppu::fetchTileNumber() {
  u8 x = ((scx + lx - 80) & 0xff) / 8;
  u8 y = ((scy + ly) & 0xff) / 8;
  u16 offset = (x + y * 32) & 0x3ff;
  u16 addr = lcdc.bgTileMapAddress(offset);
  return vram->read(addr);
}

u8 Ppu::fetchLowTileData() {
  u8 tile_number = fetchTileNumber();
  u16 base = lcdc.bgTileDataAddress(tile_number);
  u8 offset = 2 * ((ly + scy) % 8);
  return vram->read(base + offset);
}

u8 Ppu::fetchHighTileData() {
  u8 tile_number = fetchTileNumber();
  u16 base = lcdc.bgTileDataAddress(tile_number);
  u8 offset = 2 * ((ly + scy) % 8);
  return vram->read(base + offset + 1);
}

}  // namespace gbemu
