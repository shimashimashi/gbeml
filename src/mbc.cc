#include "mbc.h"

namespace gbemu {

Mbc::~Mbc(){};

u8 RomOnly::read(const u16 addr) const {
  if (addr >= 0xa000) {
    return ram[addr];
  }
  return rom.read(addr);
}

void RomOnly::write(const u16 addr, const u8 value) {
  assert(false);
  return;
}

u8 Mbc1::read(const u16 addr) const {
  if (addr >= 0 && addr <= 0x3fff) {
    return rom.read(calcRomAddress(addr));
  } else if (addr >= 0x4000 && addr <= 0x7fff) {
    return rom.read(calcRomAddress(addr));
  } else if (addr >= 0xa000 && addr <= 0xbfff) {
    if (!enable_ram) {
      assert(false);
      return 0x00;
    }

    return ram[calcRamAddress(addr)];
  } else {
    assert(false);
    return 0x00;
  }
}

void Mbc1::write(const u16 addr, const u8 value) {
  if (addr >= 0 && addr <= 0x1fff) {
    if ((value & 0xff) == 0x0a) {
      enable_ram = true;
    } else {
      enable_ram = false;
    }
  } else if (addr >= 0x2000 && addr <= 0x3fff) {
    rom_bank_number = value & 0x1f;
  } else if (addr >= 0x4000 && addr <= 0x5fff) {
    ram_bank_number = value & 0x03;
  } else if (addr >= 0x6000 && addr <= 0x7fff) {
    if (value & 1) {
      bankingMode = BankingMode::RamBankingMode;
    } else {
      bankingMode = BankingMode::SimpleRomBankingMode;
    }
  } else if (addr >= 0xa000 && addr <= 0xbfff) {
    if (!enable_ram) {
      assert(false);
      return;
    }

    ram[calcRamAddress(addr)] = value;
  } else {
    assert(false);
  }
}

u64 Mbc1::calcRomAddress(const u16 addr) const {
  u16 bank_number = 0;
  u16 offset = addr;
  if (addr >= 0 && addr <= 0x3fff) {
    if (bankingMode == BankingMode::RamBankingMode && is_large_rom) {
      bank_number = (ram_bank_number << 5);
    }
  } else {
    bank_number = (ram_bank_number << 5) + rom_bank_number;
    if (rom_bank_number == 0) {
      bank_number++;
    }
    offset -= 0x4000;
  }

  u64 rom_addr = 0x4000 * bank_number + offset;
  if (rom_addr >= rom.rom_size) {
    assert(false);
  }
  return rom_addr;
}

u64 Mbc1::calcRamAddress(const u16 addr) const {
  u64 base = 0;
  if (bankingMode == BankingMode::RamBankingMode && is_large_ram) {
    base += 0x2000 * ram_bank_number;
  }
  u64 ram_addr = base + addr - 0xa000;
  if (ram_addr >= rom.ram_size) {
    assert(false);
  }
  return ram_addr;
}

}  // namespace gbemu
