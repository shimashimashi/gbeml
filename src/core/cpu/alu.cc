#include "core/cpu/alu.h"

namespace gbeml {

u8 Alu::get_a() { return af->getHigh()->get(); }
bool Alu::get_z() { return af->getLow()->getAt(7); }
bool Alu::get_n() { return af->getLow()->getAt(6); }
bool Alu::get_h() { return af->getLow()->getAt(5); }
bool Alu::get_c() { return af->getLow()->getAt(4); }

void Alu::set_a(u8 n) { af->getHigh()->set(n); }
void Alu::set_f(u8 n) { af->getLow()->set(n); }
void Alu::set_z(bool b) { af->getLow()->setAt(7, b); }
void Alu::set_n(bool b) { af->getLow()->setAt(6, b); }
void Alu::set_h(bool b) { af->getLow()->setAt(5, b); }
void Alu::set_c(bool b) { af->getLow()->setAt(4, b); }

void Alu::add_n(u8 n) {
  u8 a = get_a();
  set_c(a + n > 0xff);
  set_h((a & 0xf) + (n & 0xf) > 0xf);
  set_z(static_cast<u8>(a + n) == 0);
  set_n(false);
  set_a(a + n);
}

void Alu::addc_n(u8 n) { return add_n(n + get_c()); }

void Alu::sub_n(u8 n) {
  u8 a = get_a();
  cp_n(n);
  set_a(a - n);
}

void Alu::subc_n(u8 n) { sub_n(n + get_c()); }

void Alu::and_n(u8 n) {
  u8 a = get_a();
  set_c(false);
  set_h(true);
  set_z((a & n) == 0);
  set_n(false);
  set_a(a & n);
}

void Alu::or_n(u8 n) {
  u8 a = get_a();
  set_c(false);
  set_h(false);
  set_z((a | n) == 0);
  set_n(false);
  set_a(a | n);
}

void Alu::xor_n(u8 n) {
  u8 a = get_a();
  set_c(false);
  set_h(false);
  set_z((a ^ n) == 0);
  set_n(false);
  set_a(a ^ n);
}

void Alu::cp_n(u8 n) {
  u8 a = get_a();
  set_c(a < n);
  set_h((a & 0xf) < (n & 0xf));
  set_z(a - n == 0);
  set_n(true);
}

void Alu::add_hl_n16(RegisterPair* r, u16 n) {
  u16 hl = r->get();
  set_c(static_cast<u32>(hl) + n > 0xffff);
  set_h((hl & 0xfff) + (n & 0xfff) > 0xfff);
  set_n(false);
  r->set(hl + n);
}

void Alu::add_sp_n8(RegisterPair* r, u8 n) {
  u16 sp = r->get();
  set_c((sp & 0xff) + n > 0xff);
  set_h((sp & 0xf) + (n & 0xf) > 0xf);
  set_z(false);
  set_n(false);
  r->set(sp + n);
}

u8 Alu::inc(u8 n) {
  set_h((n & 0xf) == 0xf);
  set_z(static_cast<u8>(n + 1) == 0);
  set_n(false);
  return n + 1;
}

u8 Alu::dec(u8 n) {
  set_h((n & 0xf) < 1);
  set_z(static_cast<u8>(n - 1) == 0);
  set_n(false);
  return n - 1;
}

u8 Alu::swap(u8 n) {
  set_z(n == 0);
  set_n(false);
  set_h(false);
  set_c(false);

  u8 h = n >> 4;
  u8 l = n & 0xf;
  l <<= 4;
  return l + h;
}

void Alu::daa() {
  u8 a = get_a();
  u8 h = a >> 4;
  u8 l = a & 0xf;

  if (get_n()) {
    if (get_c() || h > 9) {
      a -= 0x60;
    }
    if (get_h() || l > 9) {
      a -= 6;
    }

    set_c(false);
  } else {
    if (get_c() || h > 9) {
      a += 0x60;
      set_c(true);
    } else {
      set_c(false);
    }

    if (get_h() || l > 9) {
      a += 6;
    }
  }

  set_z(a == 0);
  set_h(false);
  set_a(a);
}

void Alu::cpl() {
  u8 a = get_a();
  set_n(true);
  set_h(true);
  set_a(~a);
}

void Alu::ccf() {
  set_n(false);
  set_h(false);
  set_c(!get_c());
}

void Alu::scf() {
  set_n(false);
  set_h(false);
  set_c(true);
}

void Alu::rlca() {
  u8 n = rotateLeft(get_a());
  set_a(n);
}

void Alu::rla() {
  u8 n = rotateLeftThroughCarry(get_a());
  set_a(n);
}

void Alu::rrca() {
  u8 n = rotateRight(get_a());
  set_a(n);
}

void Alu::rra() {
  u8 n = rotateRightThroughCarry(get_a());
  set_a(n);
}

u8 Alu::rlc(u8 n) { return rotateLeft(n); }

u8 Alu::rl(u8 n) { return rotateLeftThroughCarry(n); }

u8 Alu::rrc(u8 n) { return rotateRight(n); }

u8 Alu::rr(u8 n) { return rotateRightThroughCarry(n); }

u8 Alu::sla(u8 n) { return shiftLeft(n); }

u8 Alu::sra(u8 n) { return shiftRightArithmetic(n); }

u8 Alu::srl(u8 n) { return shiftRightLogical(n); }

void Alu::bit_b(u8 i, u8 n) {
  set_z(((n >> i) & 1) == 0);
  set_n(false);
  set_h(true);
}

u8 Alu::set_b(u8 i, u8 n) { return setBit(i, n); }

u8 Alu::res_b(u8 i, u8 n) { return resetBit(i, n); }

u8 Alu::rotateLeft(u8 n) {
  u8 res = static_cast<u8>(n << 1) | (n >> 7);
  set_z(res == 0);
  set_n(false);
  set_h(false);
  set_c(n >> 7);
  return res;
}

u8 Alu::rotateLeftThroughCarry(u8 n) {
  u8 res = static_cast<u8>(n << 1) | get_c();
  set_z(res == 0);
  set_n(false);
  set_h(false);
  set_c(n >> 7);
  return res;
}

u8 Alu::rotateRight(u8 n) {
  u8 res = (n >> 1) | static_cast<u8>(n << 7);
  set_z(res == 0);
  set_n(false);
  set_h(false);
  set_c(n & 1);
  return res;
}

u8 Alu::rotateRightThroughCarry(u8 n) {
  u8 res = (n >> 1) | static_cast<u8>(get_c() << 7);
  set_z(res == 0);
  set_n(false);
  set_h(false);
  set_c(n & 1);
  return res;
}

u8 Alu::shiftLeft(u8 n) {
  u8 res = static_cast<u8>(n << 1);
  set_z(res == 0);
  set_n(false);
  set_h(false);
  set_c(n >> 7);
  return res;
}

u8 Alu::shiftRightArithmetic(u8 n) {
  u8 res = (n >> 1) | (n & 0b10000000);
  set_z(res == 0);
  set_n(false);
  set_h(false);
  set_c(n & 1);
  return res;
}

u8 Alu::shiftRightLogical(u8 n) {
  u8 res = n >> 1;
  set_z(res == 0);
  set_n(false);
  set_h(false);
  set_c(n & 1);
  return res;
}

u8 Alu::setBit(u8 i, u8 n) {
  u8 b = 1;
  b <<= i;
  return n | b;
}

u8 Alu::resetBit(u8 i, u8 n) {
  u8 b = 1;
  b <<= i;
  return n & ~b;
}

}  // namespace gbeml
