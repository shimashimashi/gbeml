#include "core/memory/rom.h"

#include <glog/logging.h>

#include <cassert>
#include <fstream>

namespace gbeml {

u8 Rom::read(const u16 addr) const {
  if (addr >= data.size()) {
    DCHECK(false);
    return 0x00;
  }
  return data[addr];
}

void Rom::load(const std::string &filename) {
  std::ifstream fin(filename, std::ios::in | std::ios::binary);
  if (!fin) {
    DCHECK(false);
    return;
  }

  u8 byte;
  while (!fin.eof()) {
    char c;
    fin.read(&c, sizeof(char));
    byte = static_cast<u8>(c);
    if (fin.eof()) break;
    data.push_back(byte);
  }
  fin.close();
}

bool Rom::isValid() {
  if (data.size() < 336) {
    DCHECK(false);
    return false;
  }

  if (data.size() != getRomSize()) {
    DCHECK(false);
    return false;
  }

  if (!verifyHeaderChecksum()) {
    DCHECK(false);
    return false;
  }

  return true;
}

bool Rom::verifyHeaderChecksum() {
  u8 x = 0;
  for (u16 i = 0x134; i <= 0x14c; ++i) {
    x = x - data[i] - 1;
  }
  if (x != getHeaderChecksum()) {
    DCHECK(false);
    return false;
  }
  return true;
}

u32 Rom::getRomSize() const {
  u64 num_banks = 0;
  switch (data[0x148]) {
    case 0x00:
      num_banks = 2;
      break;
    case 0x01:
      num_banks = 4;
      break;
    case 0x02:
      num_banks = 8;
      break;
    case 0x03:
      num_banks = 16;
      break;
    case 0x04:
      num_banks = 32;
      break;
    case 0x05:
      num_banks = 64;
      break;
    case 0x06:
      num_banks = 128;
      break;
    case 0x07:
      num_banks = 257;
      break;
    case 0x08:
      num_banks = 512;
      break;
    case 0x52:
      num_banks = 72;
      break;
    case 0x53:
      num_banks = 80;
      break;
    case 0x54:
      num_banks = 96;
      break;
    default:
      DCHECK(false);
      break;
  }

  return static_cast<u32>(16 * 1024 * num_banks);
}

u32 Rom::getRamSize() const {
  u64 num_banks = 0;

  switch (data[0x149]) {
    case 0:
      num_banks = 0;
      break;
    case 2:
      num_banks = 1;
      break;
    case 3:
      num_banks = 4;
      break;
    case 4:
      num_banks = 16;
      break;
    case 5:
      num_banks = 8;
      break;
    default:
      DCHECK(false);
      break;
  }

  return static_cast<u32>(8 * 1024 * num_banks);
}

u8 Rom::getHeaderChecksum() const { return data[0x14d]; }

CartridgeType Rom::getCartridgeType() const {
  switch (data[0x147]) {
    case 0x00:
      return CartridgeType::RomOnly;
    case 0x01:
    case 0x02:
    case 0x03:
      return CartridgeType::Mbc1;
    default:
      DCHECK(false);
      return CartridgeType::RomOnly;
  }
}

}  // namespace gbeml
