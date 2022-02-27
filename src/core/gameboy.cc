#include "gameboy.h"

#include "core/bus/bus_impl.h"
#include "core/memory/ram_impl.h"

namespace gbemu {

void GameBoy::tick() {
  cpu->tick();
  ppu->tick();
  bus->tick();
}

bool GameBoy::init(const std::string& filename) {
  ic = new InterruptController(0xe1, 0x00);

  rom = new Rom();
  rom->load(filename);
  if (!rom->isValid()) {
    return false;
  }

  switch (rom->cartridgeType) {
    case CartridgeType::RomOnly:
      mbc = new RomOnly(*rom);
      break;
    case CartridgeType::Mbc1:
      mbc = new Mbc1(*rom);
      break;
  }

  hram = new RamImpl(128);
  wram = new RamImpl(8 * 1024);
  vram = new RamImpl(8 * 1024);
  oam = new RamImpl(160);

  ppu = new Ppu(display, vram, oam, ic);
  bus = new BusImpl(mbc, wram, hram, ppu, ic);
  cpu = new Cpu(bus, ic);

  cpu->set_a(0x01);
  cpu->set_f(0x80);
  cpu->set_b(0x00);
  cpu->set_c(0x13);
  cpu->set_d(0x00);
  cpu->set_e(0xd8);
  cpu->set_h(0x01);
  cpu->set_l(0x4d);
  cpu->set_pc(0x0100);
  cpu->set_sp(0xfffe);

  ppu->writeLcdc(0x91);
  ppu->writeLcdStat(0x81);
  ppu->writeScy(0x00);
  ppu->writeScx(0x00);
  ppu->writeLy(0x91);
  ppu->writeLyc(0x00);
  ppu->writeWy(0x00);
  ppu->writeWx(0x00);
  ppu->writeBgp(0xfc);

  return true;
}

}  // namespace gbemu
