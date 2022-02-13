#ifndef GBEMU_MBC_H_
#define GBEMU_MBC_H_

#include <array>

#include "rom.h"
#include "types.h"

namespace gbemu {

enum class BankingMode { SimpleRomBankingMode, RamBankingMode };

class Mbc {
 public:
  virtual ~Mbc() = 0;
  virtual u8 read(const u16 addr) const = 0;
  virtual void write(const u16 addr, const u8 value) = 0;
};

class RomOnly : public Mbc {
 public:
  RomOnly(const Rom& rom) : rom(rom){};
  ~RomOnly(){};
  u8 read(const u16 addr) const;
  void write(const u16 addr, const u8 value);

 private:
  const Rom& rom;
  std::array<u8, 8 * 1024> ram;
};

class Mbc1 : public Mbc {
 public:
  Mbc1(const Rom& rom) : rom(rom) {
    if (rom.rom_size >= (1 << 30)) {
      is_large_rom = true;
    }
    if (rom.ram_size > 8 * 1024) {
      is_large_ram = true;
    }
  };
  u8 read(const u16 addr) const;
  void write(const u16 addr, const u8 value);

 private:
  u64 calcRomAddress(const u16 addr) const;
  u64 calcRamAddress(const u16 addr) const;
  const Rom& rom;
  std::array<u8, 32 * 1024> ram;
  bool enable_ram = false;
  u8 rom_bank_number = 1;
  u8 ram_bank_number = 0;
  BankingMode bankingMode = BankingMode::SimpleRomBankingMode;
  bool is_large_ram = false;
  bool is_large_rom = false;
};

}  // namespace gbemu

#endif  // GBEMU_MBC_H_
