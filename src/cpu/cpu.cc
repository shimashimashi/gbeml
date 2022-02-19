#include "cpu/cpu.h"

#include <cstdio>

namespace gbemu {

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

void Cpu::tick() {
  if (stalls > 0) {
    stalls--;
    return;
  }

  u8 byte = fetch();
  Opcode* opcode = new Opcode(byte);
  execute(*opcode);
}

u8 Cpu::fetch() {
  u8 value = readMemory(pc->get());
  pc->increment();
  return value;
}

void Cpu::execute(const Opcode& opcode) {
  if (opcode.match("00000000")) {
    nop();
  } else if (opcode.match("00xx0001")) {
    load_r_n16(opcode);
  } else if (opcode.match("00xx0010")) {
    load_r_a(opcode);
  } else if (opcode.match("00xx0011")) {
    inc_r16(opcode);
  } else if (opcode.match("00xxx100")) {
    inc_r8(opcode);
  } else if (opcode.match("00xxx101")) {
    dec_r8(opcode);
  } else if (opcode.match("00xxx110")) {
    load_r_n8(opcode);
  } else if (opcode.match("00000111")) {
    rlca();
  } else if (opcode.match("00001000")) {
    load_n16_sp();
  } else if (opcode.match("00xx1001")) {
    add_hl_r(opcode);
  } else if (opcode.match("00xx1010")) {
    load_a_r(opcode);
  } else if (opcode.match("00xx1011")) {
    dec_r16(opcode);
  } else if (opcode.match("00001111")) {
    rrca();
  } else if (opcode.match("00010000")) {
    stop();
  } else if (opcode.match("00010111")) {
    rla();
  } else if (opcode.match("00011000")) {
    jr_n();
  } else if (opcode.match("00011111")) {
    rra();
  } else if (opcode.match("001xx000")) {
    jr_cc_n();
  } else if (opcode.match("00100111")) {
    daa();
  } else if (opcode.match("00101111")) {
    cpl();
  } else if (opcode.match("00110111")) {
    scf();
  } else if (opcode.match("00111111")) {
    ccf();
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
  } else if (opcode.match("110xx000")) {
    ret_cc();
  } else if (opcode.match("11xx0001")) {
    pop(opcode);
  } else if (opcode.match("110xx010")) {
    jp_cc_n16();
  } else if (opcode.match("11000011")) {
    jp_n16();
  } else if (opcode.match("11001011")) {
    Opcode* next = new Opcode(fetch());
    execute_cb(*next);
  } else if (opcode.match("110xx100")) {
    call_cc_n16();
  } else if (opcode.match("11xx0101")) {
    push(opcode);
  } else if (opcode.match("11000110")) {
    add_a_n();
  } else if (opcode.match("11xxx111")) {
    rst_n();
  } else if (opcode.match("11001001")) {
    ret();
  } else if (opcode.match("11001101")) {
    call_n16();
  } else if (opcode.match("11001110")) {
    addc_a_n();
  } else if (opcode.match("11010110")) {
    sub_a_n();
  } else if (opcode.match("11011001")) {
    reti();
  } else if (opcode.match("11011110")) {
    subc_a_n();
  } else if (opcode.match("11100000")) {
    load_n_a();
  } else if (opcode.match("11100010")) {
    load_c_a();
  } else if (opcode.match("11000110")) {
    and_a_n();
  } else if (opcode.match("11101000")) {
    add_sp_n();
  } else if (opcode.match("11101001")) {
    jp_hl();
  } else if (opcode.match("11101110")) {
    xor_a_n();
  } else if (opcode.match("11110000")) {
    load_a_n();
  } else if (opcode.match("11110010")) {
    load_a_c();
  } else if (opcode.match("11110011")) {
    di();
  } else if (opcode.match("11110110")) {
    or_a_n();
  } else if (opcode.match("11111000")) {
    load_hl_sp_n8();
  } else if (opcode.match("11111001")) {
    load_sp_hl();
  } else if (opcode.match("11111011")) {
    ei();
  } else {
    assert(false);
    fprintf(stderr, "opcode %x not implemented.\n", opcode.get());
  }
}

void Cpu::execute_cb(const Opcode& opcode) {
  if (opcode.match("00000xxx")) {
    rlc_n(opcode);
  } else if (opcode.match("00001xxx")) {
    rrc_n(opcode);
  } else if (opcode.match("00010xxx")) {
    rl_n(opcode);
  } else if (opcode.match("00011xxx")) {
    rr_n(opcode);
  } else if (opcode.match("00100xxx")) {
    sla_n(opcode);
  } else if (opcode.match("00101xxx")) {
    sra_n(opcode);
  } else if (opcode.match("00110xxx")) {
    swap(opcode);
  } else if (opcode.match("00111xxx")) {
    srl_n(opcode);
  } else if (opcode.match("01xxxyyy")) {
    bit_b_r(opcode);
  } else if (opcode.match("10000xxx")) {
    res_b_r(opcode);
  } else if (opcode.match("11000xxx")) {
    set_b_r(opcode);
  } else {
    assert(false);
    fprintf(stderr, "opcode %x not implemented.\n", opcode.get());
  }
}

void Cpu::load_r_n8(const Opcode& opcode) {
  u8 r = opcode.slice(3, 5);
  u8 n = fetch();
  writeRegister(r, n);
}

// 01xxxyyy
void Cpu::load_r_r(const Opcode& opcode) {
  u8 r1 = opcode.slice(3, 5);
  u8 r2 = opcode.slice(0, 2);
  u8 n = readRegister(r2);
  writeRegister(r1, n);
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
  alu->add_n(readRegister(r));
}

// 11000110
void Cpu::add_a_n() {
  u8 n = fetch();
  alu->add_n(n);
}

// 10001xxx
void Cpu::addc_a_r(const Opcode& opcode) {
  u8 r = opcode.slice(0, 2);
  alu->addc_n(readRegister(r), get_c());
}

// 10010xxx
void Cpu::sub_a_r(const Opcode& opcode) {
  u8 r = opcode.slice(0, 2);
  alu->sub_n(readRegister(r));
}

// 10011xxx
void Cpu::subc_a_r(const Opcode& opcode) {
  u8 r = opcode.slice(0, 2);
  alu->subc_n(readRegister(r), get_c());
}

// 11010110
void Cpu::sub_a_n() {
  u8 n = fetch();
  alu->sub_n(n);
}

// 11100110
void Cpu::and_a_n() {
  u8 n = fetch();
  alu->and_n(n);
}

// 10100xxx
void Cpu::and_a_r(const Opcode& opcode) {
  u8 r = opcode.slice(0, 2);
  alu->and_n(readRegister(r));
}

// 10110xxx
void Cpu::or_a_r(const Opcode& opcode) {
  u8 r = opcode.slice(0, 2);
  alu->or_n(readRegister(r));
}

// 11110110
void Cpu::or_a_n() {
  u8 n = fetch();
  alu->or_n(n);
}

// 10101xxx
void Cpu::xor_a_r(const Opcode& opcode) {
  u8 r = opcode.slice(0, 2);
  alu->xor_n(readRegister(r));
}

// 11101110
void Cpu::xor_a_n() {
  u8 n = fetch();
  alu->xor_n(n);
}

// 10111xxx
void Cpu::cp_a_r(const Opcode& opcode) {
  u8 r = opcode.slice(0, 2);
  alu->cp_n(readRegister(r));
}

// 00xxx100
void Cpu::inc_r8(const Opcode& opcode) {
  u8 r = opcode.slice(3, 5);
  if (r == 6) {
    alu->inc_memory(get_hl(), bus);
    stalls += 8;
  } else {
    alu->inc_r(selectRegister(r));
  }
}

// 00xxx101
void Cpu::dec_r8(const Opcode& opcode) {
  u8 r = opcode.slice(3, 5);
  if (r == 6) {
    alu->dec_memory(get_hl(), bus);
    stalls += 8;
  } else {
    alu->dec_r(selectRegister(r));
  }
}

// 00xx1001
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
      alu->add_hl_n16(hl, get_sp());
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
      bc->increment();
      break;
    case 1:
      de->increment();
      break;
    case 2:
      hl->increment();
      break;
    case 3:
      sp->increment();
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
      bc->decrement();
      break;
    case 1:
      de->decrement();
      break;
    case 2:
      hl->decrement();
      break;
    case 3:
      sp->decrement();
      break;
    default:
      assert(false);
      break;
  }
}

// CB + 00110xxx
void Cpu::swap(const Opcode& opcode) {
  fprintf(stderr, "opcode %x not implemented.\n", opcode.get());
}

// 00100111
void Cpu::daa() {}

// 00101111
void Cpu::cpl() {}

// 00111111
void Cpu::ccf() {}

// 00110111
void Cpu::scf() {}

// 00000000
void Cpu::nop() {}

// 01110110
void Cpu::halt() {
  fprintf(stderr, "halt not implemented.\n");
  return;
}

// 10 + 00000000
void Cpu::stop() {}

// 11110011
void Cpu::di() {}

// 11111011
void Cpu::ei() {}

// 00000111
void Cpu::rlca() {}

// 00010111
void Cpu::rla() {}

// 00001111
void Cpu::rrca() {}

// 00011111
void Cpu::rra() {}

// CB + 00000xxx
void Cpu::rlc_n(const Opcode& opcode) {
  fprintf(stderr, "opcode %x not implemented.\n", opcode.get());
}

// CB + 00010xxx
void Cpu::rl_n(const Opcode& opcode) {
  fprintf(stderr, "opcode %x not implemented.\n", opcode.get());
}

// CB + 00001xxx
void Cpu::rrc_n(const Opcode& opcode) {
  fprintf(stderr, "opcode %x not implemented.\n", opcode.get());
}

// CB + 00011xxx
void Cpu::rr_n(const Opcode& opcode) {
  fprintf(stderr, "opcode %x not implemented.\n", opcode.get());
}

// CB + 00100xxx
void Cpu::sla_n(const Opcode& opcode) {
  fprintf(stderr, "opcode %x not implemented.\n", opcode.get());
}

// CB + 00101xxx
void Cpu::sra_n(const Opcode& opcode) {
  fprintf(stderr, "opcode %x not implemented.\n", opcode.get());
}

// CB + 00111xxx
void Cpu::srl_n(const Opcode& opcode) {
  fprintf(stderr, "opcode %x not implemented.\n", opcode.get());
}

// CB + 01xxxyyy
void Cpu::bit_b_r(const Opcode& opcode) {
  fprintf(stderr, "opcode %x not implemented.\n", opcode.get());
}

// CB + 11000xxx
void Cpu::set_b_r(const Opcode& opcode) {
  fprintf(stderr, "opcode %x not implemented.\n", opcode.get());
}

// CB + 10000xxx
void Cpu::res_b_r(const Opcode& opcode) {
  fprintf(stderr, "opcode %x not implemented.\n", opcode.get());
}

// 11000011
void Cpu::jp_n16() {}

// 110xx010
void Cpu::jp_cc_n16() {}

// 11101001
void Cpu::jp_hl() {}

// 00011000
void Cpu::jr_n() {}

// 001xx000
void Cpu::jr_cc_n() {}

// 11001101
void Cpu::call_n16() {}

// 110xx100
void Cpu::call_cc_n16() {}

// 11xxx111
void Cpu::rst_n() {}

// 11001001
void Cpu::ret() {}

// 110xx000
void Cpu::ret_cc() {}

// 11011001
void Cpu::reti() {}

u8 Cpu::readRegister(u8 r) {
  if (r == 6) {
    return readMemory(get_hl());
  } else {
    return selectRegister(r)->get();
  }
}

void Cpu::writeRegister(u8 r, u8 n) {
  if (r == 6) {
    writeMemory(get_hl(), n);
  } else {
    selectRegister(r)->set(n);
  }
}

Register* Cpu::selectRegister(u8 r) {
  switch (r) {
    case 0:
      return b;
    case 1:
      return c;
    case 2:
      return d;
    case 3:
      return e;
    case 4:
      return h;
    case 5:
      return l;
    case 7:
      return a;
    default:
      assert(false);
      return nullptr;
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
