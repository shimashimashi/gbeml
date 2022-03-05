#include "core/memory/rom.h"

#include <cassert>
#include <cstdio>
#include <fstream>

namespace gbeml {

u8 Rom::read(const u16 addr) const {
  if (addr >= rom_size) {
    assert(false);
  }
  return data[addr];
}

void Rom::load(const std::string &filename) {
  std::ifstream fin(filename, std::ios::in | std::ios::binary);
  if (!fin) {
    fprintf(stderr, "Cannot read file: %s.\n", filename.c_str());
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

  setHeaders();
}

bool Rom::isValid() {
  if (data.size() != rom_size) {
    fprintf(stderr, "Cartridge size is invalid. actual = %zu, expected = %u.\n",
            data.size(), rom_size);
    return false;
  }

  if (!verifyHeaderChecksum()) {
    return false;
  }

  return true;
}

bool Rom::verifyHeaderChecksum() {
  u8 x = 0;
  for (u16 i = 0x134; i <= 0x14c; ++i) {
    x = x - data[i] - 1;
  }
  if (x != header_checksum) {
    fprintf(stderr, "Invalid Header checksum. actual = %x, expected = %x.\n", x,
            header_checksum);
    return false;
  }
  return true;
}

void Rom::setHeaders() {
  if (data.size() < 336) {
    fprintf(stderr, "Cartridge size too short: %zu bytes.\n", data.size());
    return;
  }

  for (u64 i = 0; i < 4; ++i) {
    entry_point[i] = data[0x100 + i];
  }

  for (u64 i = 0; i < 49; ++i) {
    nintendo_logo[i] = data[0x104 + i];
  }

  for (u64 i = 0; i < 17; ++i) {
    title[i] = data[0x134 + i];
  }

  for (u64 i = 0; i < 4; ++i) {
    manufacturer_code[i] = data[0x13f + i];
  }

  cgb_flag = data[0x143];

  for (u64 i = 0; i < 2; ++i) {
    new_licensee_code[i] = data[0x144 + i];
  }

  sgb_flag = data[0x146];

  setCartridgeType(data[0x147]);

  setRomSize(data[0x148]);

  setRamSize(data[0x149]);

  destination_code = data[0x14a];

  old_licensee_code = data[0x14b];

  mask_rom_version_number = data[0x14c];

  header_checksum = data[0x14d];

  for (u64 i = 0; i < 2; ++i) {
    global_checksum[i] = data[0x14e + i];
  }
}

void Rom::setRomSize(u8 code) {
  u64 num_banks = 0;
  switch (code) {
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
      fprintf(stderr, "Invalid rom size: %x.\n", code);
      assert(false);
  }

  rom_size = static_cast<u32>(16 * 1024 * num_banks);
}

void Rom::setRamSize(u8 code) {
  u64 num_banks = 0;
  switch (code) {
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
      fprintf(stderr, "Invalid ram size: %x.\n", code);
      assert(false);
      break;
  }

  ram_size = static_cast<u32>(8 * 1024 * num_banks);
}

void Rom::setCartridgeType(u8 code) {
  switch (code) {
    case 0:
      cartridgeType = CartridgeType::RomOnly;
      return;
    case 1:
      cartridgeType = CartridgeType::Mbc1;
      return;
    default:
      fprintf(stderr, "Cartridge type %x not implemented.\n", code);
      assert(false);
      return;
  }
}

}  // namespace gbeml
