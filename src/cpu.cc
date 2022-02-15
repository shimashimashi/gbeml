#include "cpu.h"

#include <cstdio>

namespace gbemu {

u16 Cpu::get_af() { return af->get(); }
u16 Cpu::get_bc() { return bc->get(); }
u16 Cpu::get_de() { return de->get(); }
u16 Cpu::get_hl() { return hl->get(); }

void Cpu::set_af(u16 n) { return af->set(n); }
void Cpu::set_bc(u16 n) { return bc->set(n); }
void Cpu::set_de(u16 n) { return de->set(n); }
void Cpu::set_hl(u16 n) { return hl->set(n); }

u8 Cpu::get_a() { return af->getHigh(); }
u8 Cpu::get_f() { return af->getLow(); }
u8 Cpu::get_b() { return bc->getHigh(); }
u8 Cpu::get_c() { return bc->getLow(); }
u8 Cpu::get_d() { return de->getHigh(); }
u8 Cpu::get_e() { return de->getLow(); }
u8 Cpu::get_h() { return hl->getHigh(); }
u8 Cpu::get_l() { return hl->getLow(); }
bool Cpu::get_z() { return af->getAt(7); }
bool Cpu::get_n() { return af->getAt(6); }
bool Cpu::get_hcy() { return af->getAt(5); }
bool Cpu::get_cy() { return af->getAt(4); }

void Cpu::set_a(u8 n) { af->setHigh(n); }
void Cpu::set_f(u8 n) { af->setLow(n); }
void Cpu::set_b(u8 n) { bc->setHigh(n); }
void Cpu::set_c(u8 n) { bc->setLow(n); }
void Cpu::set_d(u8 n) { de->setHigh(n); }
void Cpu::set_e(u8 n) { de->setLow(n); }
void Cpu::set_h(u8 n) { hl->setHigh(n); }
void Cpu::set_l(u8 n) { hl->setLow(n); }
void Cpu::set_z(bool b) { af->setAt(7, b); }
void Cpu::set_n(bool b) { af->setAt(6, b); }
void Cpu::set_hcy(bool b) { af->setAt(5, b); }
void Cpu::set_cy(bool b) { af->setAt(4, b); }

u8 Cpu::fetch() {
  u8 value = readMemory(pc->get());
  pc->increment();
  stalls += 4;
  return value;
}

void Cpu::execute(u8 opcode) {
  if (match(opcode, "01110110")) {
    halt();
  } else if (match(opcode, "00xxx110")) {
    load_r_n8(opcode);
  } else if (match(opcode, "01xxxyyy")) {
    load_r_r(opcode);
  } else if (match(opcode, "00xx0010")) {
    load_r_a(opcode);
  } else if (match(opcode, "11110010")) {
    load_a_c();
  } else if (match(opcode, "11100010")) {
    load_c_a();
  } else if (match(opcode, "11100000")) {
    load_n_a();
  } else if (match(opcode, "11110000")) {
    load_a_n();
  } else if (match(opcode, "00xx0001")) {
    load_r_n16(opcode);
  } else if (match(opcode, "11111001")) {
    load_sp_hl();
  } else if (match(opcode, "11111001")) {
    load_hl_sp_n8();
  } else if (match(opcode, "00001000")) {
    load_n16_sp();
  } else if (match(opcode, "11xx0101")) {
    push(opcode);
  } else if (match(opcode, "11xx0001")) {
    pop(opcode);
  } else {
    fprintf(stderr, "opcode %x not implemented.\n", opcode);
  }
}

// 00xxx110
void Cpu::load_r_n8(u8 opcode) {
  u8 r = (opcode & 0b00111000) >> 3;
  u8 n = fetch();
  setRegister(r, n);
}

// 01xxxyyy
void Cpu::load_r_r(u8 opcode) {
  u8 r1 = (opcode & 0b00111000) >> 3;
  u8 r2 = opcode & 0b00000111;
  u8 n = getRegister(r2);
  setRegister(r1, n);
}

// 00xx0010
void Cpu::load_r_a(u8 opcode) {
  switch (opcode >> 4) {
    case 0:
      writeMemory(get_bc(), get_a());
      break;
    case 1:
      writeMemory(get_de(), get_a());
      break;
    case 2:
      writeMemory(get_hl(), get_a());
      hl->increment();
      break;
    case 3:
      writeMemory(get_hl(), get_a());
      hl->decrement();
      break;
    default:
      assert(false);
      break;
  }
}

// 00xx1010
void Cpu::load_a_r(u8 opcode) {
  u8 value;
  switch (opcode >> 4) {
    case 0:
      value = readMemory(get_bc());
      break;
    case 1:
      value = readMemory(get_de());
      break;
    case 2:
      value = readMemory(get_hl());
      hl->increment();
      break;
    case 3:
      value = readMemory(get_hl());
      hl->decrement();
      break;
    default:
      assert(false);
      break;
  }
  set_a(value);
}

// 11110010
void Cpu::load_a_c() { set_a(readMemory(0xff00 + get_c())); }

// 11100010
void Cpu::load_c_a() { writeMemory(0xff00 + get_c(), get_a()); }

// 11100000
void Cpu::load_n_a() {
  u8 n = fetch();
  writeMemory(0xff00 + n, get_a());
}

// 11110000
void Cpu::load_a_n() {
  u8 n = fetch();
  set_a(readMemory(0xff00 + n));
}

// 00xx0001
void Cpu::load_r_n16(u8 opcode) {
  u8 n1 = fetch();
  u8 n2 = fetch();
  u16 n = concat(n2, n1);

  switch (opcode >> 4) {
    case 0:
      set_bc(n);
      break;
    case 1:
      set_de(n);
      break;
    case 2:
      set_hl(n);
      break;
    case 3:
      sp->set(n);
      break;
    default:
      assert(false);
      break;
  }
}

// 11111001
void Cpu::load_sp_hl() {
  sp->set(get_hl());
  stalls += 4;
}

// 11111000
void Cpu::load_hl_sp_n8() {
  u8 n = fetch();
  set_hl(sp->get() + n);
  set_z(false);
  set_n(false);
  set_hcy(isCarrySetAdd4(sp->getLow(), n));
  set_cy(isCarrySetAdd8(sp->getLow(), n));
  stalls += 4;
}

// 00001000
void Cpu::load_n16_sp() {
  u8 l = fetch();
  u8 h = fetch();
  u16 n = concat(h, l);

  writeMemory(n, sp->getLow());
  writeMemory(n + 1, sp->getHigh());
  stalls += 8;
}

// 11xx0101
void Cpu::push(u8 opcode) {
  u8 h;
  u8 l;
  switch (opcode & 0b00110000 >> 4) {
    case 0:
      h = get_b();
      l = get_c();
      break;
    case 1:
      h = get_d();
      l = get_e();
      break;
    case 2:
      h = get_h();
      l = get_l();
      break;
    case 3:
      h = get_a();
      l = get_f();
      break;
    default:
      assert(false);
      break;
  }
  sp->decrement();
  writeMemory(sp->get(), h);
  sp->decrement();
  writeMemory(sp->get(), l);
  stalls += 4;
}

// 11xx0001
void Cpu::pop(u8 opcode) {
  u8 l = readMemory(sp->get());
  sp->increment();
  u8 h = readMemory(sp->get());
  sp->increment();
  u16 n = concat(h, l);

  switch (opcode & 0b00110000 >> 4) {
    case 0:
      set_bc(n);
      break;
    case 1:
      set_de(n);
      break;
    case 2:
      set_hl(n);
      break;
    case 3:
      set_af(n);
      break;
    default:
      assert(false);
      break;
  }
}

// 11101010
void Cpu::load_n16_a() {}

// 11111010
void Cpu::load_a_n16() {}

void Cpu::halt() {
  fprintf(stderr, "halt not implemented.\n");
  return;
}

bool Cpu::match(u8 opcode, const std::string& pattern) {
  for (u8 i = 0; i < 8; ++i) {
    if (pattern[i] == '0' || pattern[i] == '1') {
      char bit = opcode >> (7 - i) & 1 ? '1' : '0';
      if (bit != pattern[i]) {
        return false;
      }
    }
  }
  return true;
}

u8 Cpu::getRegister(u8 r) {
  switch (r) {
    case 0:
      return get_b();
    case 1:
      return get_c();
    case 2:
      return get_d();
    case 3:
      return get_e();
    case 4:
      return get_h();
    case 5:
      return get_l();
    case 6:
      return readMemory(get_hl());
    case 7:
      return get_a();
    default:
      assert(false);
      return 0;
  }
}

void Cpu::setRegister(u8 r, u8 n) {
  switch (r) {
    case 0:
      set_b(n);
      break;
    case 1:
      set_c(n);
      break;
    case 2:
      set_d(n);
      break;
    case 3:
      set_e(n);
      break;
    case 4:
      set_h(n);
      break;
    case 5:
      set_l(n);
      break;
    case 6:
      writeMemory(get_hl(), n);
      break;
    case 7:
      set_a(n);
      break;
    default:
      assert(false);
      break;
  }
}

u8 Cpu::readMemory(u16 addr) {
  stalls += 4;
  return bus->read(addr);
}

void Cpu::writeMemory(u16 addr, u8 value) {
  stalls += 4;
  bus->write(addr, value);
}

bool Cpu::isCarrySetAdd8(u8 n1, u8 n2) {
  u16 n = static_cast<u16>(n1) + n2;
  return n > 0xff;
}

bool Cpu::isCarrySetAdd4(u8 n1, u8 n2) {
  n1 &= 0x0f;
  n2 &= 0x0f;
  u8 n = n1 + n2;
  return n > 0x0f;
}

u16 Cpu::concat(u8 n1, u8 n2) {
  return static_cast<u16>((static_cast<u16>(n1) << 8)) + n2;
}

}  // namespace gbemu
