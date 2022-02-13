#include <iostream>

#include "mbc.h"
#include "rom.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "usage: gbemu <rom>.gb" << std::endl;
    return 1;
  }
  std::string filename(argv[1]);

  gbemu::Rom* rom = new gbemu::Rom();
  rom->load(filename);
  if (!rom->isValid()) {
    return 1;
  }

  gbemu::Mbc* mbc;
  switch (rom->cartridgeType) {
    case gbemu::CartridgeType::RomOnly:
      mbc = new gbemu::RomOnly(*rom);
      break;
    case gbemu::CartridgeType::Mbc1:
      mbc = new gbemu::Mbc1(*rom);
      break;
    default:
      assert(false);
      return 1;
  }

  std::cout << "OK" << std::endl;
  return 0;
}
