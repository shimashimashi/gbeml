#include "core/cpu/cpu.h"

#include <glog/logging.h>

namespace gbemu {

u16 Cpu::get_af() { return af.get(); }
u16 Cpu::get_bc() { return bc.get(); }
u16 Cpu::get_de() { return de.get(); }
u16 Cpu::get_hl() { return hl.get(); }
u16 Cpu::get_sp() { return sp.get(); }
u16 Cpu::get_pc() { return pc.get(); }

void Cpu::set_bc(u16 n) { return bc.set(n); }
void Cpu::set_de(u16 n) { return de.set(n); }
void Cpu::set_hl(u16 n) { return hl.set(n); }
void Cpu::set_sp(u16 n) { return sp.set(n); }
void Cpu::set_pc(u16 n) { return pc.set(n); }

u8 Cpu::get_a() { return alu.get_a(); }
u8 Cpu::get_f() { return af.getLow()->get(); }
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
  if (stalled()) {
    stalls--;
    return;
  }

  if (interruptEnabled()) {
    handleInterrupt();
  }

  if (halted) {
    return;
  }

  u8 byte = fetch();
  Opcode opcode(byte);
  execute(opcode);
}

bool Cpu::stalled() { return stalls > 0; }

bool Cpu::interruptEnabled() { return ime; }

u8 Cpu::fetch() {
  u8 value = readMemory(get_pc());
  pc.increment();
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
    jr_cc_n(opcode);
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
    ret_cc(opcode);
  } else if (opcode.match("11xx0001")) {
    pop(opcode);
  } else if (opcode.match("110xx010")) {
    jp_cc_n16(opcode);
  } else if (opcode.match("11000011")) {
    jp_n16();
  } else if (opcode.match("11001011")) {
    Opcode next(fetch());
    execute_cb(next);
  } else if (opcode.match("110xx100")) {
    call_cc_n16(opcode);
  } else if (opcode.match("11xx0101")) {
    push(opcode);
  } else if (opcode.match("11000110")) {
    add_a_n();
  } else if (opcode.match("11xxx111")) {
    rst_n(opcode);
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
  } else if (opcode.match("11100110")) {
    and_a_n();
  } else if (opcode.match("11101000")) {
    add_sp_n();
  } else if (opcode.match("11101001")) {
    jp_hl();
  } else if (opcode.match("11101010")) {
    load_n16_a();
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
  } else if (opcode.match("11111010")) {
    load_a_n16();
  } else if (opcode.match("11111011")) {
    ei();
  } else if (opcode.match("11111110")) {
    cp_a_n();
  } else {
    DLOG(WARNING) << "opcode " << opcode.get() << " not implemented."
                  << std::endl;
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
  } else if (opcode.match("10xxxyyy")) {
    res_b_r(opcode);
  } else if (opcode.match("11xxxyyy")) {
    set_b_r(opcode);
  } else {
    DCHECK(false);
  }
}

void Cpu::load_r_n8(const Opcode& opcode) {
  u8 r = opcode.slice(3, 5);
  u8 n = fetch();
  VLOG(1) << "ld r, 0x" << std::hex << int(n) << std::endl;
  writeRegister(r, n);
}

// 01xxxyyy
void Cpu::load_r_r(const Opcode& opcode) {
  u8 r1 = opcode.slice(3, 5);
  u8 r2 = opcode.slice(0, 2);
  u8 n = readRegister(r2);
  VLOG(1) << "ld r, r" << std::endl;
  writeRegister(r1, n);
}

// 00xx0010
void Cpu::load_r_a(const Opcode& opcode) {
  switch (opcode.slice(4, 5)) {
    case 0:
      VLOG(1) << "ld [bc], a" << std::endl;
      writeMemory(get_bc(), get_a());
      break;
    case 1:
      VLOG(1) << "ld [de], a" << std::endl;
      writeMemory(get_de(), get_a());
      break;
    case 2:
      VLOG(1) << "ld [hl+], a" << std::endl;
      writeMemory(get_hl(), get_a());
      hl.increment();
      break;
    case 3:
      VLOG(1) << "ld [hl-], a" << std::endl;
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
      VLOG(1) << "ld a, [bc]" << std::endl;
      value = readMemory(get_bc());
      break;
    case 1:
      VLOG(1) << "ld a, [de]" << std::endl;
      value = readMemory(get_de());
      break;
    case 2:
      VLOG(1) << "ld a, [hl+]" << std::endl;
      value = readMemory(get_hl());
      hl.increment();
      break;
    case 3:
      VLOG(1) << "ld a, [hl-]" << std::endl;
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
  VLOG(1) << "ld [0xff" << std::hex << int(n) << "], a" << std::endl;
  writeMemory(0xff00 + n, get_a());
}

// 11110000
void Cpu::load_a_n() {
  u8 n = fetch();
  VLOG(1) << "ld a, [0xff" << std::hex << int(n) << "]" << std::endl;
  set_a(readMemory(0xff00 + n));
}

// 00xx0001
// cycles = 12
void Cpu::load_r_n16(const Opcode& opcode) {
  u16 n = fetchWord();
  u8 r = opcode.slice(4, 5);
  VLOG(1) << "ld r, 0x" << std::hex << int(n) << std::endl;
  selectBcDeHlSp(r)->set(n);
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
  alu.add_sp_n8(&sp, fetch());
  set_hl(get_sp());
  stalls += 4;
}

// 00001000
// cycles = 20
void Cpu::load_n16_sp() {
  u16 word = fetchWord();
  VLOG(1) << "ld [sp], 0x" << std::hex << int(word) << std::endl;
  writeWord(get_sp(), word);
}

// 11xx0101
// cycles = 16
void Cpu::push(const Opcode& opcode) {
  u8 r = opcode.slice(4, 5);
  u16 word = selectBcDeHlAf(r)->get();
  pushStack(word);
  stalls += 4;
}

// 11xx0001
// cycles = 12
void Cpu::pop(const Opcode& opcode) {
  u16 word = popStack();
  u8 r = opcode.slice(4, 5);
  selectBcDeHlAf(r)->set(word);
}

// 11101010
// cycles = 16
void Cpu::load_n16_a() {
  u16 addr = fetchWord();
  VLOG(1) << "ld [0x" << std::hex << int(addr) << "], a" << std::endl;
  writeMemory(addr, get_a());
}

// 11111010
// cycles = 16
void Cpu::load_a_n16() {
  u16 addr = fetchWord();
  VLOG(1) << "ld a, [0x" << std::hex << int(addr) << "]" << std::endl;
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
  VLOG(1) << "cp a, 0x" << std::hex << int(n) << std::endl;
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
  u16 n = selectBcDeHlSp(r)->get();
  alu.add_hl_n16(&hl, n);
  stalls += 4;
}

// 11101000
void Cpu::add_sp_n() {
  u8 n = fetch();
  alu.add_sp_n8(&sp, n);
  stalls += 8;
}

// 00xx0011
void Cpu::inc_r16(const Opcode& opcode) {
  u8 r = opcode.slice(4, 5);
  selectBcDeHlSp(r)->increment();
  VLOG(1) << "inc16" << std::endl;
}

// 00xx1011
void Cpu::dec_r16(const Opcode& opcode) {
  u8 r = opcode.slice(4, 5);
  selectBcDeHlSp(r)->decrement();
  VLOG(1) << "dec16" << std::endl;
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
}

// 110xx010
void Cpu::jp_cc_n16(const Opcode& opcode) {
  u16 addr = fetchWord();
  u8 n = opcode.slice(3, 4);
  if (checkFlags(n)) {
    set_pc(addr);
  }
}

// 11101001
void Cpu::jp_hl() { set_pc(get_hl()); }

// 00011000
void Cpu::jr_n() {
  u8 offset = fetch();
  jumpRelative(offset);
}

// 001xx000
void Cpu::jr_cc_n(const Opcode& opcode) {
  u8 offset = fetch();
  u8 n = opcode.slice(3, 4);
  if (checkFlags(n)) {
    jumpRelative(offset);
  }
}

// 11001101
// cycles = 12
void Cpu::call_n16() {
  u16 addr = fetchWord();
  call(addr);
  stalls -= 8;
}

// 110xx100
// cycles = 12
void Cpu::call_cc_n16(const Opcode& opcode) {
  u16 addr = fetchWord();
  u8 n = opcode.slice(3, 4);
  if (checkFlags(n)) {
    call(addr);
  }
  // not sure
  stalls -= 8;
}

// 11xxx111
// cycles = 32
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
  stalls += 20;
}

// 11001001
// cycles = 8
void Cpu::ret() {
  u16 addr = popStack();
  set_pc(addr);
  stalls -= 4;
}

// 110xx000
void Cpu::ret_cc(const Opcode& opcode) {
  u8 n = opcode.slice(3, 4);
  if (checkFlags(n)) {
    ret();
  }
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

RegisterPair* Cpu::selectBcDeHlSp(u8 r) {
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

RegisterPair* Cpu::selectBcDeHlAf(u8 r) {
  switch (r) {
    case 0:
      return &bc;
    case 1:
      return &de;
    case 2:
      return &hl;
    case 3:
      return &af;
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

void Cpu::handleInterrupt() {
  if (ic->isVBlankRequested() && ic->isVBlankEnabled()) {
    call(0x40);
    stalls += 12;
    halted = false;
    ime = false;
    ic->clearVBlank();
  } else if (ic->isLcdStatRequested() && ic->isLcdStatEnabled()) {
    call(0x48);
    stalls += 12;
    halted = false;
    ime = false;
    ic->clearLcdStat();
  }
}

}  // namespace gbemu
