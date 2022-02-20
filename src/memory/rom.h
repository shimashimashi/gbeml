#ifndef GBEMU_ROM_H_
#define GBEMU_ROM_H_

#include <string>
#include <vector>

#include "types.h"

namespace gbemu {

enum class CartridgeType { RomOnly, Mbc1 };

class Rom {
 public:
  u8 read(const u16 addr) const;
  void load(const std::string &filename);
  bool isValid();

  u8 entry_point[4];
  u8 nintendo_logo[48];
  u8 title[16];
  u8 manufacturer_code[4];
  u8 cgb_flag;
  u8 new_licensee_code[2];
  u8 sgb_flag;
  CartridgeType cartridgeType;
  u32 rom_size;
  u32 ram_size;
  u8 destination_code;
  u8 old_licensee_code;
  u8 mask_rom_version_number;
  u8 header_checksum;
  u8 global_checksum[2];

 private:
  void setHeaders();
  bool verifyHeaderChecksum();
  void setRomSize(u8 code);
  void setRamSize(u8 code);
  void setCartridgeType(u8 code);

  std::vector<u8> data;
};

}  // namespace gbemu

#endif  // GBEMU_ROM_H_
