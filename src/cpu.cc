#include "cpu.h"

#include <cstdio>

namespace gbemu {

void Cpu::tick() {
  if (stalls > 0) {
    stalls--;
    return;
  }

  u8 byte = fetch();
  Opcode* opcode = new Opcode(byte);
  execute(*opcode);
}

u16 Cpu::get_af() { return af->get(); }
u16 Cpu::get_bc() { return bc->get(); }
u16 Cpu::get_de() { return de->get(); }
u16 Cpu::get_hl() { return hl->get(); }
u16 Cpu::get_sp() { return sp->get(); }

void Cpu::set_af(u16 n) { return af->set(n); }
void Cpu::set_bc(u16 n) { return bc->set(n); }
void Cpu::set_de(u16 n) { return de->set(n); }
void Cpu::set_hl(u16 n) { return hl->set(n); }
void Cpu::set_sp(u16 n) { return sp->set(n); }

u8 Cpu::get_a() { return a->get(); }
u8 Cpu::get_f() { return f->get(); }
u8 Cpu::get_b() { return b->get(); }
u8 Cpu::get_c() { return c->get(); }
u8 Cpu::get_d() { return d->get(); }
u8 Cpu::get_e() { return e->get(); }
u8 Cpu::get_h() { return h->get(); }
u8 Cpu::get_l() { return l->get(); }

void Cpu::set_a(u8 n) { a->set(n); }
void Cpu::set_f(u8 n) { f->set(n); }
void Cpu::set_b(u8 n) { b->set(n); }
void Cpu::set_c(u8 n) { c->set(n); }
void Cpu::set_d(u8 n) { d->set(n); }
void Cpu::set_e(u8 n) { e->set(n); }
void Cpu::set_h(u8 n) { h->set(n); }
void Cpu::set_l(u8 n) { l->set(n); }

void Cpu::set_z(bool i) { f->setAt(3, i); }
void Cpu::set_n(bool i) { f->setAt(2, i); }
void Cpu::set_hcy(bool i) { f->setAt(1, i); }
void Cpu::set_cy(bool i) { f->setAt(0, i); }

u8 Cpu::fetch() {
  u8 value = readMemory(pc->get());
  pc->increment();
  return value;
}

void Cpu::execute(const Opcode& opcode) {
  if (opcode.match("00001000")) {
    load_n16_sp();
  } else if (opcode.match("00xx0001")) {
    load_r_n16(opcode);
  } else if (opcode.match("00xx0010")) {
    load_r_a(opcode);
  } else if (opcode.match("00xx0011")) {
    inc_r16(opcode);
  } else if (opcode.match("00xx1001")) {
    add_hl_r(opcode);
  } else if (opcode.match("00xx1011")) {
    dec_r16(opcode);
  } else if (opcode.match("00xxx110")) {
    load_r_n8(opcode);
  } else if (opcode.match("01110110")) {
    halt();
  } else if (opcode.match("01xxxyyy")) {
    load_r_r(opcode);
  } else if (opcode.match("10000xxx")) {
    add_a_r(opcode);
  } else if (opcode.match("10001xxx")) {
    addc_a_r(opcode);
  } else if (opcode.match("10010xxx")) {
    sub_a_r(opcode);
  } else if (opcode.match("10011xxx")) {
    subc_a_r(opcode);
  } else if (opcode.match("10100xxx")) {
    and_a_r(opcode);
  } else if (opcode.match("10101xxx")) {
    xor_a_r(opcode);
  } else if (opcode.match("10110xxx")) {
    or_a_r(opcode);
  } else if (opcode.match("10111xxx")) {
    cp_a_r(opcode);
  } else if (opcode.match("00xxx100")) {
    inc_r8(opcode);
  } else if (opcode.match("00xxx101")) {
    dec_r8(opcode);
  } else if (opcode.match("11100010")) {
    load_c_a();
  } else if (opcode.match("11100000")) {
    load_n_a();
  } else if (opcode.match("11101000")) {
    add_sp_n();
  } else if (opcode.match("11110000")) {
    load_a_n();
  } else if (opcode.match("11110010")) {
    load_a_c();
  } else if (opcode.match("11111000")) {
    load_hl_sp_n8();
  } else if (opcode.match("11111001")) {
    load_sp_hl();
  } else if (opcode.match("11xx0101")) {
    push(opcode);
  } else if (opcode.match("11xx0001")) {
    pop(opcode);
  } else {
    fprintf(stderr, "opcode %x not implemented.\n", opcode.get());
  }
}

void Cpu::load_r_n8(const Opcode& opcode) {
  u8 r = opcode.slice(3, 5);
  u8 n = fetch();
  writeHalfRegister(r, n);
}

// 01xxxyyy
void Cpu::load_r_r(const Opcode& opcode) {
  u8 r1 = opcode.slice(3, 5);
  u8 r2 = opcode.slice(0, 2);
  u8 n = readHalfRegister(r2);
  writeHalfRegister(r1, n);
}

// 00xx0010
void Cpu::load_r_a(const Opcode& opcode) {
  switch (opcode.slice(4, 5)) {
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
void Cpu::load_a_r(const Opcode& opcode) {
  u8 value;
  switch (opcode.slice(4, 5)) {
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
// cycles = 12
void Cpu::load_r_n16(const Opcode& opcode) {
  u16 n = fetchWord();
  switch (opcode.slice(4, 5)) {
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
      set_sp(n);
      break;
    default:
      assert(false);
      break;
  }
}

// 11111001
// cycles = 8
void Cpu::load_sp_hl() {
  set_sp(get_hl());
  stalls += 4;
}

// 11111000
// cycles = 12
void Cpu::load_hl_sp_n8() {
  alu->add_sp_n8(sp, fetch());
  set_hl(get_sp());
  stalls += 4;
}

// 00001000
// cycles = 20
void Cpu::load_n16_sp() {
  u16 word = fetchWord();
  writeWord(get_sp(), word);
}

// 11xx0101
// cycles = 16
void Cpu::push(const Opcode& opcode) {
  u16 word;
  switch (opcode.slice(4, 5)) {
    case 0:
      word = get_bc();
      break;
    case 1:
      word = get_de();
      break;
    case 2:
      word = get_hl();
      break;
    case 3:
      word = get_af();
      break;
    default:
      assert(false);
      break;
  }

  sp->decrement();
  sp->decrement();
  writeWord(get_sp(), word);
  stalls += 4;
}

// 11xx0001
// cycles = 12
void Cpu::pop(const Opcode& opcode) {
  u16 word = readWord(get_sp());
  sp->increment();
  sp->increment();

  switch (opcode.slice(4, 5)) {
    case 0:
      set_bc(word);
      break;
    case 1:
      set_de(word);
      break;
    case 2:
      set_hl(word);
      break;
    case 3:
      set_af(word);
      break;
    default:
      assert(false);
      break;
  }
}

// 11101010
// cycles = 16
void Cpu::load_n16_a() {
  u16 addr = fetchWord();
  writeMemory(addr, get_a());
}

// 11111010
// cycles = 16
void Cpu::load_a_n16() {
  u16 addr = fetchWord();
  set_a(readMemory(addr));
}

// 10000xxx
void Cpu::add_a_r(const Opcode& opcode) {
  u8 r = opcode.slice(0, 2);
  alu->add_n(readHalfRegister(r));
}

// 10001xxx
void Cpu::addc_a_r(const Opcode& opcode) {
  u8 r = opcode.slice(0, 2);
  alu->addc_n(readHalfRegister(r), get_c());
}

// 10010xxx
void Cpu::sub_a_r(const Opcode& opcode) {
  u8 r = opcode.slice(0, 2);
  alu->sub_n(readHalfRegister(r));
}

// 10011xxx
void Cpu::subc_a_r(const Opcode& opcode) {
  u8 r = opcode.slice(0, 2);
  alu->subc_n(readHalfRegister(r), get_c());
}

// 10100xxx
void Cpu::and_a_r(const Opcode& opcode) {
  u8 r = opcode.slice(0, 2);
  alu->and_n(readHalfRegister(r));
}

// 10110xxx
void Cpu::or_a_r(const Opcode& opcode) {
  u8 r = opcode.slice(0, 2);
  alu->or_n(readHalfRegister(r));
}

// 10101xxx
void Cpu::xor_a_r(const Opcode& opcode) {
  u8 r = opcode.slice(0, 2);
  alu->xor_n(readHalfRegister(r));
}

// 10111xxx
void Cpu::cp_a_r(const Opcode& opcode) {
  u8 r = opcode.slice(0, 2);
  alu->cp_n(readHalfRegister(r));
}

// 00xxx100
void Cpu::inc_r8(const Opcode& opcode) {
  u8 r = opcode.slice(3, 5);
  u8 n = readHalfRegister(r);
  // Setting flags here because INC (HL) is difficult to handle inside Alu.
  set_hcy((n & 0xf) == 0xf);
  set_z(n + 1 == 0);
  set_n(false);
  writeHalfRegister(r, n + 1);
}

// 00xxx101
void Cpu::dec_r8(const Opcode& opcode) {
  u8 r = opcode.slice(3, 5);
  u8 n = readHalfRegister(r);
  set_hcy((n & 0xf) < 1);
  set_z(n - 1 == 0);
  set_n(false);
  writeHalfRegister(r, n - 1);
}

// 00xx1111
void Cpu::add_hl_r(const Opcode& opcode) {
  u8 r = opcode.slice(4, 5);
  switch (r) {
    case 0:
      alu->add_hl_n16(hl, get_bc());
      break;
    case 1:
      alu->add_hl_n16(hl, get_de());
      break;
    case 2:
      alu->add_hl_n16(hl, get_hl());
      break;
    case 3:
      alu->add_hl_n16(hl, get_hl());
      break;
    default:
      break;
  }
  stalls += 4;
}

// 11101000
void Cpu::add_sp_n() {
  u8 n = fetch();
  alu->add_sp_n8(sp, n);
  stalls += 8;
}

// 00xx0011
void Cpu::inc_r16(const Opcode& opcode) {
  u8 r = opcode.slice(4, 5);
  switch (r) {
    case 0:
      alu->inc16(bc);
      break;
    case 1:
      alu->inc16(de);
      break;
    case 2:
      alu->inc16(hl);
      break;
    case 3:
      alu->inc16(sp);
      break;
    default:
      assert(false);
      break;
  }
}

// 00xx1011
void Cpu::dec_r16(const Opcode& opcode) {
  u8 r = opcode.slice(4, 5);
  switch (r) {
    case 0:
      alu->dec16(bc);
      break;
    case 1:
      alu->dec16(de);
      break;
    case 2:
      alu->dec16(hl);
      break;
    case 3:
      alu->dec16(sp);
      break;
    default:
      assert(false);
      break;
  }
}

void Cpu::halt() {
  fprintf(stderr, "halt not implemented.\n");
  return;
}

u8 Cpu::readHalfRegister(u8 r) {
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

void Cpu::writeHalfRegister(u8 r, u8 n) {
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

u16 Cpu::readWord(u16 addr) {
  u8 low = readMemory(addr);
  u8 high = readMemory(addr + 1);
  return concat(high, low);
}

void Cpu::writeWord(u16 addr, u16 value) {
  u8 high = value >> 8;
  u8 low = (value & 0xff);
  writeMemory(addr, low);
  writeMemory(addr + 1, high);
}

u16 Cpu::fetchWord() {
  u8 low = fetch();
  u8 high = fetch();
  return concat(high, low);
}

}  // namespace gbemu
