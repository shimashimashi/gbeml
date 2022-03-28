#ifndef GBEML_MBC_H_
#define GBEML_MBC_H_

#include <array>

#include "core/memory/rom.h"
#include "core/types/types.h"

namespace gbeml {

enum class BankingMode { SimpleRomBankingMode, RamBankingMode };

class Mbc {
 public:
  virtual ~Mbc() {}

  virtual u8 readRom(const u16 addr) const = 0;
  virtual u8 readRam(const u16 addr) const = 0;
  virtual void writeRom(const u16 addr, const u8 value) = 0;
  virtual void writeRam(const u16 addr, const u8 value) = 0;
};

class RomOnly : public Mbc {
 public:
  RomOnly(const Rom& rom_) : rom(rom_) {}

  u8 readRom(const u16 addr) const override;
  u8 readRam(const u16 addr) const override;
  void writeRom(const u16 addr, const u8 value) override;
  void writeRam(const u16 addr, const u8 value) override;

 private:
  const Rom& rom;
  std::array<u8, 8 * 1024> ram;
};

class Mbc1 : public Mbc {
 public:
  Mbc1(const Rom& rom_) : rom(rom_) {
    if (rom.getRomSize() >= (1 << 30)) {
      is_large_rom = true;
    }
    if (rom.getRamSize() > 8 * 1024) {
      is_large_ram = true;
    }
  }

  u8 readRom(const u16 addr) const override;
  u8 readRam(const u16 addr) const override;
  void writeRom(const u16 addr, const u8 value) override;
  void writeRam(const u16 addr, const u8 value) override;

 private:
  u16 calcRomAddress(const u16 addr) const;
  u64 calcRamAddress(const u16 addr) const;
  const Rom& rom;
  std::array<u8, 128 * 1024> ram;
  bool enable_ram = false;
  u8 rom_bank_number = 1;
  u8 ram_bank_number = 0;
  BankingMode mode = BankingMode::SimpleRomBankingMode;
  bool is_large_ram = false;
  bool is_large_rom = false;
};

}  // namespace gbeml

#endif  // GBEML_MBC_H_
