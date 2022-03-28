#include "core/memory/mbc.h"

#include "core/log/logging.h"

#include <iostream>

namespace gbeml {

Mbc::~Mbc() {}

u8 RomOnly::readRom(const u16 addr) const { return rom.read(addr); }

u8 RomOnly::readRam(const u16 addr) const {
  DCHECK(addr < ram.size());
  return ram[addr];
}

void RomOnly::writeRom([[maybe_unused]] const u16 addr,
                       [[maybe_unused]] const u8 value) {
  DCHECK(false);
}

void RomOnly::writeRam(const u16 addr, const u8 value) { ram[addr] = value; }

u8 Mbc1::readRom(const u16 addr) const {
  return rom.read(calcRomAddress(addr));
}

u8 Mbc1::readRam(const u16 addr) const {
  if (!enable_ram) {
    DLOG(WARNING) << "Ram disabled." << std::endl;
    return 0x00;
  }
  u64 ram_addr = calcRamAddress(addr);
  DCHECK(ram_addr < ram.size());
  return ram[ram_addr];
}

void Mbc1::writeRom(const u16 addr, const u8 value) {
  if (addr <= 0x1fff) {
    if ((value & 0xff) == 0x0a) {
      enable_ram = true;
    } else {
      enable_ram = false;
    }
  } else if (addr <= 0x3fff) {
    rom_bank_number = value & 0x1f;
  } else if (addr <= 0x5fff) {
    ram_bank_number = value & 0x03;
  } else {
    if (value & 1) {
      mode = BankingMode::RamBankingMode;
    } else {
      mode = BankingMode::SimpleRomBankingMode;
    }
  }
}

void Mbc1::writeRam(const u16 addr, const u8 value) {
  if (!enable_ram) {
    DLOG(WARNING) << "Ram disabled." << std::endl;
    return;
  }
  u64 ram_addr = calcRamAddress(addr);
  DCHECK(ram_addr < ram.size());
  ram[ram_addr] = value;
}

u16 Mbc1::calcRomAddress(const u16 addr) const {
  if (addr <= 0x3fff) {
    if (mode == BankingMode::RamBankingMode && is_large_rom) {
      u16 bank_number = static_cast<u16>(ram_bank_number << 5);
      return 0x4000 * bank_number + addr;
    } else {
      return addr;
    }
  } else {
    u16 bank_number = static_cast<u16>(ram_bank_number << 5) + rom_bank_number;
    if (rom_bank_number == 0) {
      bank_number++;
    }
    return 0x4000 * bank_number + addr - 0x4000;
  }
}

u64 Mbc1::calcRamAddress(const u16 addr) const {
  u64 base = 0;
  if (mode == BankingMode::RamBankingMode && is_large_ram) {
    base += 0x2000 * ram_bank_number;
  }
  return base + addr;
}

}  // namespace gbeml
