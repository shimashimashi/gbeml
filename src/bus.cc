#include "bus.h"

#include <cstdio>

namespace gbemu {

u8 Bus::read(u16 addr) const {
  if (addr <= 0x7fff) {
    return mbc->read(addr);
  } else if (addr <= 0x9fff) {
    fprintf(stderr, "Not implemented to read %x\n", addr);
    return 0x00;
  } else if (addr <= 0xbfff) {
    return mbc->read(addr);
  } else if (addr <= 0xdfff) {
    return wram[addr - 0xc000];
  } else if (addr <= 0xfdff) {
    fprintf(stderr, "Not implemented to read %x\n", addr);
    return 0x00;
  } else if (addr <= 0xfe9f) {
    fprintf(stderr, "Not implemented to read %x\n", addr);
    return 0x00;
  } else if (addr <= 0xfeff) {
    fprintf(stderr, "Not implemented to read %x\n", addr);
    return 0x00;
  } else if (addr <= 0xff7f) {
    fprintf(stderr, "Not implemented to read %x\n", addr);
    return 0x00;
  } else if (addr <= 0xfffe) {
    return hram[addr - 0xff80];
  } else {
    fprintf(stderr, "Not implemented to read %x\n", addr);
    return 0x00;
  }
}

void Bus::write(u16 addr, u8 value) {
  if (addr <= 0x7fff) {
    mbc->write(addr, value);
  } else if (addr <= 0x9fff) {
    fprintf(stderr, "Not implemented to write %x\n", addr);
  } else if (addr <= 0xbfff) {
    mbc->write(addr, value);
  } else if (addr <= 0xdfff) {
    wram[addr - 0xc000] = value;
  } else if (addr <= 0xfdff) {
    fprintf(stderr, "Not implemented to write %x\n", addr);
  } else if (addr <= 0xfe9f) {
    fprintf(stderr, "Not implemented to write %x\n", addr);
  } else if (addr <= 0xfeff) {
    fprintf(stderr, "Not implemented to write %x\n", addr);
  } else if (addr <= 0xff7f) {
    fprintf(stderr, "Not implemented to write %x\n", addr);
  } else if (addr <= 0xfffe) {
    hram[addr - 0xff80] = value;
  } else {
    fprintf(stderr, "Not implemented to write %x\n", addr);
  }
}

}  // namespace gbemu