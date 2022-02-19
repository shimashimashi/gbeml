#include "cpu/alu.h"

namespace gbemu {

u8 Alu::get_a() { return af->getHigh()->get(); }
bool Alu::get_z() { return af->getLow()->getAt(7); }
bool Alu::get_n() { return af->getLow()->getAt(6); }
bool Alu::get_h() { return af->getLow()->getAt(5); }
bool Alu::get_c() { return af->getLow()->getAt(4); }

void Alu::set_a(u8 n) { af->getHigh()->set(n); }
void Alu::set_z(bool b) { af->getLow()->setAt(7, b); }
void Alu::set_n(bool b) { af->getLow()->setAt(6, b); }
void Alu::set_h(bool b) { af->getLow()->setAt(5, b); }
void Alu::set_c(bool b) { af->getLow()->setAt(4, b); }

void Alu::add_n(u8 n) {
  u8 a = get_a();
  set_c(static_cast<u16>(a) + n > 0xff);
  set_h((a & 0xf) + (n & 0xf) > 0xf);
  set_z(static_cast<u8>(a + n) == 0);
  set_n(false);
  set_a(a + n);
}

void Alu::addc_n(u8 n, bool carry) { return add_n(n + carry); }

void Alu::sub_n(u8 n) {
  u8 a = get_a();
  cp_n(n);
  set_a(a - n);
}

void Alu::subc_n(u8 n, bool carry) { sub_n(n + carry); }

void Alu::and_n(u8 n) {
  u8 a = get_a();
  set_c(false);
  set_h(true);
  set_z((a & n) == 0);
  set_n(false);
  set_a(a &= n);
}

void Alu::or_n(u8 n) {
  u8 a = get_a();
  set_c(false);
  set_h(false);
  set_z((a | n) == 0);
  set_n(false);
  set_a(a |= n);
}

void Alu::xor_n(u8 n) {
  u8 a = get_a();
  set_c(false);
  set_h(false);
  set_z((a ^ n) == 0);
  set_n(false);
  set_a(a ^= n);
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

void Alu::inc_r(Register* r) {
  u8 n = r->get();
  setFlagsInc(n);
  r->set(n + 1);
}

void Alu::dec_r(Register* r) {
  u8 n = r->get();
  setFlagsDec(n);
  r->set(n - 1);
}

void Alu::inc_memory(u16 addr, Bus* bus) {
  u8 n = bus->read(addr);
  setFlagsInc(n);
  bus->write(addr, n + 1);
}

void Alu::dec_memory(u16 addr, Bus* bus) {
  u8 n = bus->read(addr);
  setFlagsDec(n);
  bus->write(addr, n - 1);
}

void Alu::swap_r(Register* r) {
  u8 n = r->get();
  setFlagsSwap(n);
  r->set(swap(n));
}

void Alu::swap_memory(u16 addr, Bus* bus) {
  u8 n = bus->read(addr);
  setFlagsSwap(n);
  bus->write(addr, swap(n));
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

void Alu::setFlagsInc(u8 n) {
  set_h((n & 0xf) == 0xf);
  set_z(static_cast<u8>(n + 1) == 0);
  set_n(false);
}

void Alu::setFlagsDec(u8 n) {
  set_h((n & 0xf) < 1);
  set_z(static_cast<u8>(n - 1) == 0);
  set_n(false);
}

void Alu::setFlagsSwap(u8 n) {
  set_z(n == 0);
  set_n(false);
  set_h(false);
  set_c(false);
}

u8 Alu::swap(u8 n) {
  u8 h = n >> 4;
  u8 l = n & 0xf;
  l <<= 4;
  return l + h;
}

}  // namespace gbemu
