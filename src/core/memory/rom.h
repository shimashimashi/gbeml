#ifndef GBEML_ROM_H_
#define GBEML_ROM_H_

#include <string>
#include <vector>

#include "core/types/types.h"

namespace gbeml {

enum class CartridgeType { RomOnly, Mbc1 };

class Rom {
 public:
  u8 read(const u16 addr) const;
  void load(const std::string &filename);
  bool isValid();
  u32 getRomSize() const;
  u32 getRamSize() const;
  CartridgeType getCartridgeType() const;

 private:
  bool verifyHeaderChecksum();
  u8 getHeaderChecksum() const;

  std::vector<u8> data;
};

}  // namespace gbeml

#endif  // GBEML_ROM_H_
