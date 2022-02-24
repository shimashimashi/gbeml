#include "bus/bus_impl.h"

#include <cstdio>

namespace gbemu {

u8 BusImpl::read(u16 addr) const {
  if (addr <= 0x7fff) {
    return mbc->read(addr);
  } else if (addr <= 0x97ff) {
    return ppu->readTileData(addr);
  } else if (addr <= 0x9fff) {
    return ppu->readTileMap(addr);
  } else if (addr <= 0xbfff) {
    return mbc->read(addr);
  } else if (addr <= 0xdfff) {
    return wram->read(addr);
  } else if (addr <= 0xfdff) {
    fprintf(stderr, "Not implemented to read %x\n", addr);
    return 0x00;
  } else if (addr <= 0xfe9f) {
    return ppu->readOam(addr);
  } else if (addr <= 0xfeff) {
    fprintf(stderr, "Address %x is not usable\n", addr);
    return 0x00;
  } else if (addr == 0xff0f) {
    return ic->readIf();
  } else if (addr == 0xff40) {
    return ppu->readLcdc();
  } else if (addr == 0xff41) {
    return ppu->readLcdStat();
  } else if (addr == 0xff42) {
    return ppu->readScy();
  } else if (addr == 0xff43) {
    return ppu->readScx();
  } else if (addr == 0xff44) {
    return ppu->readLy();
  } else if (addr == 0xff45) {
    return ppu->readLyc();
  } else if (addr == 0xff46) {
    return static_cast<u8>(dma_source_address >> 8);
  } else if (addr == 0xff47) {
    return ppu->readBgp();
  } else if (addr == 0xff48) {
    return ppu->readObp0();
  } else if (addr == 0xff49) {
    return ppu->readObp1();
  } else if (addr == 0xff4a) {
    return ppu->readWy();
  } else if (addr == 0xff4b) {
    return ppu->readWx();
  } else if (addr <= 0xff7f) {
    fprintf(stderr, "Not implemented to read %x\n", addr);
    return 0x00;
  } else if (addr <= 0xfffe) {
    return hram->read(addr);
  } else if (addr == 0xffff) {
    return ic->readIe();
  } else {
    fprintf(stderr, "Not implemented to read %x\n", addr);
    return 0x00;
  }
}

void BusImpl::write(u16 addr, u8 value) {
  if (addr <= 0x7fff) {
    mbc->write(addr, value);
  } else if (addr <= 0x97ff) {
    ppu->writeTileData(addr, value);
  } else if (addr <= 0x9fff) {
    ppu->writeTileMap(addr, value);
  } else if (addr <= 0xbfff) {
    mbc->write(addr, value);
  } else if (addr <= 0xdfff) {
    wram->write(addr, value);
  } else if (addr <= 0xfdff) {
    fprintf(stderr, "Not implemented to write %x\n", addr);
  } else if (addr <= 0xfe9f) {
    ppu->writeOam(addr, value);
  } else if (addr <= 0xfeff) {
    fprintf(stderr, "Address %x is not usable\n", addr);
  } else if (addr == 0xff0f) {
    ic->writeIf(value);
  } else if (addr == 0xff40) {
    ppu->writeLcdc(value);
  } else if (addr == 0xff41) {
    ppu->writeLcdStat(value);
  } else if (addr == 0xff42) {
    ppu->writeScy(value);
  } else if (addr == 0xff43) {
    ppu->writeScx(value);
  } else if (addr == 0xff44) {
    ppu->writeLy(value);
  } else if (addr == 0xff45) {
    ppu->writeLyc(value);
  } else if (addr == 0xff46) {
    dma(value);
  } else if (addr == 0xff47) {
    ppu->writeBgp(value);
  } else if (addr == 0xff48) {
    ppu->writeObp0(value);
  } else if (addr == 0xff49) {
    ppu->writeObp1(value);
  } else if (addr == 0xff4a) {
    ppu->writeWy(value);
  } else if (addr == 0xff4b) {
    ppu->writeWx(value);
  } else if (addr <= 0xff7f) {
    fprintf(stderr, "Not implemented to write %x\n", addr);
  } else if (addr <= 0xfffe) {
    hram->write(addr, value);
  } else if (addr == 0xffff) {
    ic->writeIe(value);
  } else {
    fprintf(stderr, "Not implemented to write %x\n", addr);
  }
}

void BusImpl::tick() {
  if (stalls > 0) {
    stalls--;
    return;
  }

  if (!is_transfering) {
    return;
  }

  transfer();

  stalls += 4;
}

void BusImpl::dma(u8 value) {
  is_transfering = true;
  dma_source_address = concat(value, 0x00);
}

void BusImpl::transfer() {
  u16 addr = concat(0xfe, static_cast<u8>(dma_source_address & 0xff));
  ppu->writeOam(addr, read(dma_source_address));
  dma_source_address++;
  if ((dma_source_address & 0xff) == 0xff) {
    is_transfering = false;
  }
}

}  // namespace gbemu
