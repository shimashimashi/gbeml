#include <iostream>

#include "gameboy.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "usage: gbemu <rom>.gb" << std::endl;
    return 1;
  }
  std::string filename(argv[1]);

  gbemu::GameBoy gb;
  gb.init(filename);

  std::cout << "OK" << std::endl;
  return 0;
}
