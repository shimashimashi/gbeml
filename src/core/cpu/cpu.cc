#include "core/cpu/cpu.h"

#include <debugbreak.h>
#include <glog/logging.h>

namespace gbeml {

u16 Cpu::get_af() { return af.get() & 0xfff0; }
u16 Cpu::get_bc() { return bc.get(); }
u16 Cpu::get_de() { return de.get(); }
u16 Cpu::get_hl() { return hl.get(); }
u16 Cpu::get_sp() { return sp.get(); }
u16 Cpu::get_pc() { return pc.get(); }

void Cpu::set_af(u16 n) { return af.set(n); }
void Cpu::set_bc(u16 n) { return bc.set(n); }
void Cpu::set_de(u16 n) { return de.set(n); }
void Cpu::set_hl(u16 n) { return hl.set(n); }
void Cpu::set_sp(u16 n) { return sp.set(n); }
void Cpu::set_pc(u16 n) { return pc.set(n); }

u8 Cpu::get_a() { return alu.get_a(); }
u8 Cpu::get_b() { return bc.getHigh()->get(); }
u8 Cpu::get_c() { return bc.getLow()->get(); }
u8 Cpu::get_d() { return de.getHigh()->get(); }
u8 Cpu::get_e() { return de.getLow()->get(); }
u8 Cpu::get_h() { return hl.getHigh()->get(); }
u8 Cpu::get_l() { return hl.getLow()->get(); }
bool Cpu::get_carry() { return alu.get_c(); }
bool Cpu::get_z() { return alu.get_z(); }

void Cpu::set_a(u8 n) { alu.set_a(n); }
void Cpu::set_f(u8 n) { alu.set_f(n); }
void Cpu::set_b(u8 n) { bc.getHigh()->set(n); }
void Cpu::set_c(u8 n) { bc.getLow()->set(n); }
void Cpu::set_d(u8 n) { de.getHigh()->set(n); }
void Cpu::set_e(u8 n) { de.getLow()->set(n); }
void Cpu::set_h(u8 n) { hl.getHigh()->set(n); }
void Cpu::set_l(u8 n) { hl.getLow()->set(n); }
void Cpu::set_carry(bool flag) { alu.set_c(flag); }
void Cpu::set_z(bool flag) { alu.set_z(flag); }

void Cpu::tick() {
  if (isStalled()) {
    stalls--;
    return;
  }

  if (breakpoint > -1 && breakpoint == get_pc()) {
    debug_break();
  }

  if (ic->isInterruptRequested()) {
    halted = false;
    if (interruptEnabled()) {
      handleInterrupt();
      stalls--;
      return;
    }
  }

  if (isHalted()) {
    return;
  }

  u8 byte = fetch();
  Opcode opcode(byte);
  execute(opcode);
  stalls--;
}

void Cpu::advance(u64 n) {
  for (u64 i = 0; i < n; ++i) {
    tick();
  }
}

bool Cpu::isStalled() { return stalls > 0; }

bool Cpu::isHalted() { return halted; }

bool Cpu::interruptEnabled() { return ime; }

u8 Cpu::fetch() {
  u8 value = readMemory(get_pc());
  pc.increment();
  return value;
}

void Cpu::execute(const Opcode& opcode) {
  u8 byte = opcode.get();

  if (byte == 0b00000000) {
    nop();
  } else if ((byte & 0b11001111) == 0b00000001) {
    load_r_n16(opcode);
  } else if ((byte & 0b11001111) == 0b00000010) {
    load_r_a(opcode);
  } else if ((byte & 0b11001111) == 0b00000011) {
    inc_r16(opcode);
  } else if ((byte & 0b11000111) == 0b00000100) {
    inc_r8(opcode);
  } else if ((byte & 0b11000111) == 0b00000101) {
    dec_r8(opcode);
  } else if ((byte & 0b11000111) == 0b00000110) {
    load_r_n8(opcode);
  } else if (byte == 0b00000111) {
    rlca();
  } else if (byte == 0b00001000) {
    load_n16_sp();
  } else if ((byte & 0b11001111) == 0b00001001) {
    add_hl_r(opcode);
  } else if ((byte & 0b11001111) == 0b00001010) {
    load_a_r(opcode);
  } else if ((byte & 0b11001111) == 0b00001011) {
    dec_r16(opcode);
  } else if (byte == 0b00001111) {
    rrca();
  } else if (byte == 0b00010000) {
    stop();
  } else if (byte == 0b00010111) {
    rla();
  } else if (byte == 0b00011000) {
    jr_n();
  } else if (byte == 0b00011111) {
    rra();
  } else if ((byte & 0b11100111) == 0b00100000) {
    jr_cc_n(opcode);
  } else if (byte == 0b00100111) {
    daa();
  } else if (byte == 0b00101111) {
    cpl();
  } else if (byte == 0b00110111) {
    scf();
  } else if (byte == 0b00111111) {
    ccf();
  } else if (byte == 0b01110110) {
    halt();
  } else if ((byte & 0b11000000) == 0b01000000) {
    load_r_r(opcode);
  } else if ((byte & 0b11111000) == 0b10000000) {
    add_a_r(opcode);
  } else if ((byte & 0b11111000) == 0b10001000) {
    addc_a_r(opcode);
  } else if ((byte & 0b11111000) == 0b10010000) {
    sub_a_r(opcode);
  } else if ((byte & 0b11111000) == 0b10011000) {
    subc_a_r(opcode);
  } else if ((byte & 0b11111000) == 0b10100000) {
    and_a_r(opcode);
  } else if ((byte & 0b11111000) == 0b10101000) {
    xor_a_r(opcode);
  } else if ((byte & 0b11111000) == 0b10110000) {
    or_a_r(opcode);
  } else if ((byte & 0b11111000) == 0b10111000) {
    cp_a_r(opcode);
  } else if ((byte & 0b11100111) == 0b11000000) {
    ret_cc(opcode);
  } else if ((byte & 0b11001111) == 0b11000001) {
    pop(opcode);
  } else if ((byte & 0b11100111) == 0b11000010) {
    jp_cc_n16(opcode);
  } else if (byte == 0b11000011) {
    jp_n16();
  } else if (byte == 0b11001011) {
    Opcode next(fetch());
    execute_cb(next);
  } else if ((byte & 0b11100111) == 0b11000100) {
    call_cc_n16(opcode);
  } else if ((byte & 0b11001111) == 0b11000101) {
    push(opcode);
  } else if (byte == 0b11000110) {
    add_a_n();
  } else if ((byte & 0b11000111) == 0b11000111) {
    rst_n(opcode);
  } else if (byte == 0b11001001) {
    ret();
  } else if (byte == 0b11001101) {
    call_n16();
  } else if (byte == 0b11001110) {
    addc_a_n();
  } else if (byte == 0b11010110) {
    sub_a_n();
  } else if (byte == 0b11011001) {
    reti();
  } else if (byte == 0b11011110) {
    subc_a_n();
  } else if (byte == 0b11100000) {
    load_n_a();
  } else if (byte == 0b11100010) {
    load_c_a();
  } else if (byte == 0b11100110) {
    and_a_n();
  } else if (byte == 0b11101000) {
    add_sp_n();
  } else if (byte == 0b11101001) {
    jp_hl();
  } else if (byte == 0b11101010) {
    load_n16_a();
  } else if (byte == 0b11101110) {
    xor_a_n();
  } else if (byte == 0b11110000) {
    load_a_n();
  } else if (byte == 0b11110010) {
    load_a_c();
  } else if (byte == 0b11110011) {
    di();
  } else if (byte == 0b11110110) {
    or_a_n();
  } else if (byte == 0b11111000) {
    load_hl_sp_n8();
  } else if (byte == 0b11111001) {
    load_sp_hl();
  } else if (byte == 0b11111010) {
    load_a_n16();
  } else if (byte == 0b11111011) {
    ei();
  } else if (byte == 0b11111110) {
    cp_a_n();
  } else {
    DLOG(WARNING) << "opcode " << opcode.get() << " not implemented."
                  << std::endl;
  }
}

void Cpu::execute_cb(const Opcode& opcode) {
  u8 byte = opcode.get();

  if ((byte & 0b11111000) == 0b00000000) {
    rlc_n(opcode);
  } else if ((byte & 0b11111000) == 0b00001000) {
    rrc_n(opcode);
  } else if ((byte & 0b11111000) == 0b00010000) {
    rl_n(opcode);
  } else if ((byte & 0b11111000) == 0b00011000) {
    rr_n(opcode);
  } else if ((byte & 0b11111000) == 0b00100000) {
    sla_n(opcode);
  } else if ((byte & 0b11111000) == 0b00101000) {
    sra_n(opcode);
  } else if ((byte & 0b11111000) == 0b00110000) {
    swap(opcode);
  } else if ((byte & 0b11111000) == 0b00111000) {
    srl_n(opcode);
  } else if ((byte & 0b11000000) == 0b01000000) {
    bit_b_r(opcode);
  } else if ((byte & 0b11000000) == 0b10000000) {
    res_b_r(opcode);
  } else if ((byte & 0b11000000) == 0b11000000) {
    set_b_r(opcode);
  } else {
    DCHECK(false);
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
      hl.increment();
      break;
    case 3:
      writeMemory(get_hl(), get_a());
      hl.decrement();
      break;
    default:
      DCHECK(false);
      break;
  }
}

// 00xx1010
void Cpu::load_a_r(const Opcode& opcode) {
  u8 value = 0;
  switch (opcode.slice(4, 5)) {
    case 0:
      value = readMemory(get_bc());
      break;
    case 1:
      value = readMemory(get_de());
      break;
    case 2:
      value = readMemory(get_hl());
      hl.increment();
      break;
    case 3:
      value = readMemory(get_hl());
      hl.decrement();
      break;
    default:
      DCHECK(false);
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
  u8 r = opcode.slice(4, 5);
  selectRegisterPair(r)->set(n);
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
  u16 word = alu.add_sp_n8(&sp, fetch());
  set_hl(word);
  stalls += 4;
}

// 00001000
// cycles = 20
void Cpu::load_n16_sp() {
  u16 word = fetchWord();
  writeWord(word, get_sp());
}

// 11xx0101
// cycles = 16
void Cpu::push(const Opcode& opcode) {
  u8 r = opcode.slice(4, 5);
  u16 word = 0;

  switch (r) {
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
      DCHECK(false);
  }

  pushStack(word);
  stalls += 4;
}

// 11xx0001
// cycles = 12
void Cpu::pop(const Opcode& opcode) {
  u16 word = popStack();
  u8 r = opcode.slice(4, 5);

  switch (r) {
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
      DCHECK(false);
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
  alu.add_n(readRegister(r));
}

// 11000110
void Cpu::add_a_n() {
  u8 n = fetch();
  alu.add_n(n);
}

// 10001xxx
void Cpu::addc_a_r(const Opcode& opcode) {
  u8 r = opcode.slice(0, 2);
  alu.addc_n(readRegister(r));
}

void Cpu::addc_a_n() { alu.addc_n(fetch()); }

// 10010xxx
void Cpu::sub_a_r(const Opcode& opcode) {
  u8 r = opcode.slice(0, 2);
  alu.sub_n(readRegister(r));
}

// 10011xxx
void Cpu::subc_a_r(const Opcode& opcode) {
  u8 r = opcode.slice(0, 2);
  alu.subc_n(readRegister(r));
}

void Cpu::subc_a_n() { alu.subc_n(fetch()); }

// 11010110
void Cpu::sub_a_n() {
  u8 n = fetch();
  alu.sub_n(n);
}

// 11100110
void Cpu::and_a_n() {
  u8 n = fetch();
  alu.and_n(n);
}

// 10100xxx
void Cpu::and_a_r(const Opcode& opcode) {
  u8 r = opcode.slice(0, 2);
  alu.and_n(readRegister(r));
}

// 10110xxx
void Cpu::or_a_r(const Opcode& opcode) {
  u8 r = opcode.slice(0, 2);
  alu.or_n(readRegister(r));
}

// 11110110
void Cpu::or_a_n() {
  u8 n = fetch();
  alu.or_n(n);
}

// 10101xxx
void Cpu::xor_a_r(const Opcode& opcode) {
  u8 r = opcode.slice(0, 2);
  alu.xor_n(readRegister(r));
}

// 11101110
void Cpu::xor_a_n() {
  u8 n = fetch();
  alu.xor_n(n);
}

// 10111xxx
void Cpu::cp_a_r(const Opcode& opcode) {
  u8 r = opcode.slice(0, 2);
  alu.cp_n(readRegister(r));
}

// 11111110
void Cpu::cp_a_n() {
  u8 n = fetch();
  alu.cp_n(n);
}

// 00xxx100
void Cpu::inc_r8(const Opcode& opcode) {
  u8 r = opcode.slice(3, 5);
  u8 n = alu.inc(readRegister(r));
  writeRegister(r, n);
}

// 00xxx101
void Cpu::dec_r8(const Opcode& opcode) {
  u8 r = opcode.slice(3, 5);
  u8 n = alu.dec(readRegister(r));
  writeRegister(r, n);
}

// 00xx1001
void Cpu::add_hl_r(const Opcode& opcode) {
  u8 r = opcode.slice(4, 5);
  u16 n = selectRegisterPair(r)->get();
  alu.add_hl_n16(&hl, n);
  stalls += 4;
}

// 11101000
void Cpu::add_sp_n() {
  u8 n = fetch();
  u16 word = alu.add_sp_n8(&sp, static_cast<i8>(n));
  set_sp(word);
  stalls += 8;
}

// 00xx0011
void Cpu::inc_r16(const Opcode& opcode) {
  u8 r = opcode.slice(4, 5);
  selectRegisterPair(r)->increment();
  stalls += 4;
}

// 00xx1011
void Cpu::dec_r16(const Opcode& opcode) {
  u8 r = opcode.slice(4, 5);
  selectRegisterPair(r)->decrement();
  stalls += 4;
}

// CB + 00110xxx
void Cpu::swap(const Opcode& opcode) {
  u8 r = opcode.slice(0, 3);
  u8 n = alu.swap(readRegister(r));
  writeRegister(r, n);
}

// 00100111
void Cpu::daa() { alu.daa(); }

// 00101111
void Cpu::cpl() { alu.cpl(); }

// 00111111
void Cpu::ccf() { alu.ccf(); }

// 00110111
void Cpu::scf() { alu.scf(); }

// 00000000
void Cpu::nop() {}

// 01110110
void Cpu::halt() {
  halted = true;
  return;
}

// 10 + 00000000
void Cpu::stop() {
  VLOG(1) << "stop not implemented" << std::endl;
  return;
}

// 11110011
void Cpu::di() { ime = false; }

// 11111011
void Cpu::ei() { ime = true; }

// 00000111
void Cpu::rlca() { alu.rlca(); }

// 00010111
void Cpu::rla() { alu.rla(); }

// 00001111
void Cpu::rrca() { alu.rrca(); }

// 00011111
void Cpu::rra() { alu.rra(); }

// CB + 00000xxx
void Cpu::rlc_n(const Opcode& opcode) {
  u8 r = opcode.slice(0, 2);
  u8 n = alu.rlc(readRegister(r));
  writeRegister(r, n);
}

// CB + 00010xxx
void Cpu::rl_n(const Opcode& opcode) {
  u8 r = opcode.slice(0, 2);
  u8 n = alu.rl(readRegister(r));
  writeRegister(r, n);
}

// CB + 00001xxx
void Cpu::rrc_n(const Opcode& opcode) {
  u8 r = opcode.slice(0, 2);
  u8 n = alu.rrc(readRegister(r));
  writeRegister(r, n);
}

// CB + 00011xxx
void Cpu::rr_n(const Opcode& opcode) {
  u8 r = opcode.slice(0, 2);
  u8 n = alu.rr(readRegister(r));
  writeRegister(r, n);
}

// CB + 00100xxx
void Cpu::sla_n(const Opcode& opcode) {
  u8 r = opcode.slice(0, 2);
  u8 n = alu.sla(readRegister(r));
  writeRegister(r, n);
}

// CB + 00101xxx
void Cpu::sra_n(const Opcode& opcode) {
  u8 r = opcode.slice(0, 2);
  u8 n = alu.sra(readRegister(r));
  writeRegister(r, n);
}

// CB + 00111xxx
void Cpu::srl_n(const Opcode& opcode) {
  u8 r = opcode.slice(0, 2);
  u8 n = alu.srl(readRegister(r));
  writeRegister(r, n);
}

// CB + 01xxxyyy
void Cpu::bit_b_r(const Opcode& opcode) {
  u8 i = opcode.slice(3, 5);
  u8 r = opcode.slice(0, 2);
  alu.bit_b(i, readRegister(r));
}

// CB + 11xxxyyy
void Cpu::set_b_r(const Opcode& opcode) {
  u8 i = opcode.slice(3, 5);
  u8 r = opcode.slice(0, 2);
  u8 n = alu.set_b(i, readRegister(r));
  writeRegister(r, n);
}

// CB + 10xxxyyy
void Cpu::res_b_r(const Opcode& opcode) {
  u8 i = opcode.slice(3, 5);
  u8 r = opcode.slice(0, 2);
  u8 n = alu.res_b(i, readRegister(r));
  writeRegister(r, n);
}

// 11000011
void Cpu::jp_n16() {
  u16 addr = fetchWord();
  set_pc(addr);
  stalls += 4;
}

// 110xx010
void Cpu::jp_cc_n16(const Opcode& opcode) {
  u16 addr = fetchWord();
  u8 n = opcode.slice(3, 4);
  if (checkFlags(n)) {
    set_pc(addr);
    stalls += 4;
  }
}

// 11101001
void Cpu::jp_hl() { set_pc(get_hl()); }

// 00011000
void Cpu::jr_n() {
  u8 offset = fetch();
  jumpRelative(offset);
  stalls += 4;
}

// 001xx000
void Cpu::jr_cc_n(const Opcode& opcode) {
  u8 offset = fetch();
  u8 n = opcode.slice(3, 4);
  if (checkFlags(n)) {
    jumpRelative(offset);
    stalls += 4;
  }
}

// 11001101
// cycles = 24
void Cpu::call_n16() {
  u16 addr = fetchWord();
  call(addr);
  stalls += 4;
}

// 110xx100
// cycles = 12
void Cpu::call_cc_n16(const Opcode& opcode) {
  u16 addr = fetchWord();
  u8 n = opcode.slice(3, 4);
  if (checkFlags(n)) {
    call(addr);
    stalls += 4;
  }
}

// 11xxx111
// cycles = 16
void Cpu::rst_n(const Opcode& opcode) {
  pushStack(get_pc());

  u8 addr = 0;
  u8 n = opcode.slice(3, 5);
  switch (n) {
    case 0:
      addr = 0x00;
      break;
    case 1:
      addr = 0x08;
      break;
    case 2:
      addr = 0x10;
      break;
    case 3:
      addr = 0x18;
      break;
    case 4:
      addr = 0x20;
      break;
    case 5:
      addr = 0x28;
      break;
    case 6:
      addr = 0x30;
      break;
    case 7:
      addr = 0x38;
      break;
    default:
      DCHECK(false);
      break;
  }
  set_pc(addr);
  stalls += 4;
}

// 11001001
// cycles = 16
void Cpu::ret() {
  u16 addr = popStack();
  set_pc(addr);
  stalls += 4;
}

// 110xx000
// cycles = 20 if true else 8
void Cpu::ret_cc(const Opcode& opcode) {
  u8 n = opcode.slice(3, 4);
  if (checkFlags(n)) {
    ret();
  }
  stalls += 4;
}

// 11011001
void Cpu::reti() {
  ret();
  ime = true;
}

u8 Cpu::readRegister(u8 r) {
  switch (r) {
    case 0:
      return bc.getHigh()->get();
    case 1:
      return bc.getLow()->get();
    case 2:
      return de.getHigh()->get();
    case 3:
      return de.getLow()->get();
    case 4:
      return hl.getHigh()->get();
    case 5:
      return hl.getLow()->get();
    case 6:
      return readMemory(hl.get());
    case 7:
      return af.getHigh()->get();
    default:
      DCHECK(false);
      return 0;
  }
}

void Cpu::writeRegister(u8 r, u8 n) {
  switch (r) {
    case 0:
      return bc.getHigh()->set(n);
    case 1:
      return bc.getLow()->set(n);
    case 2:
      return de.getHigh()->set(n);
    case 3:
      return de.getLow()->set(n);
    case 4:
      return hl.getHigh()->set(n);
    case 5:
      return hl.getLow()->set(n);
    case 6:
      return writeMemory(hl.get(), n);
    case 7:
      return af.getHigh()->set(n);
    default:
      DCHECK(false);
      return;
  }
}

RegisterPair* Cpu::selectRegisterPair(u8 r) {
  switch (r) {
    case 0:
      return &bc;
    case 1:
      return &de;
    case 2:
      return &hl;
    case 3:
      return &sp;
    default:
      DCHECK(false);
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

void Cpu::jumpRelative(u8 offset) {
  set_pc(static_cast<u16>(get_pc() + static_cast<i8>(offset)));
}

bool Cpu::checkFlags(u8 n) {
  switch (n) {
    case 0:
      return !alu.get_z();
    case 1:
      return alu.get_z();
    case 2:
      return !alu.get_c();
    case 3:
      return alu.get_c();
    default:
      DCHECK(false);
      return false;
  }
}

void Cpu::pushStack(u16 word) {
  sp.decrement();
  sp.decrement();
  writeWord(get_sp(), word);
}

u16 Cpu::popStack() {
  u16 word = readWord(get_sp());
  sp.increment();
  sp.increment();
  return word;
}

void Cpu::call(u16 addr) {
  pushStack(get_pc());
  set_pc(addr);
}

// cycles = 20
void Cpu::handleInterrupt() {
  stalls += 12;
  ime = false;

  if (ic->isVBlankRequested()) {
    ic->clearVBlank();
    call(0x40);
  } else if (ic->isLcdStatRequested()) {
    ic->clearLcdStat();
    call(0x48);
  } else if (ic->isTimerRequested()) {
    ic->clearTimer();
    call(0x50);
  } else if (ic->isSerialRequested()) {
    ic->clearSerial();
    call(0x58);
  } else if (ic->isJoypadRequested()) {
    ic->clearJoypad();
    call(0x60);
  }
}

void Cpu::setBreakpoint(i32 value) { breakpoint = value; }

}  // namespace gbeml
