#include "alu.h"

namespace gbemu {

u8 Alu::get_a() { return a_->get(); }
bool Alu::get_z() { return f_->getAt(3); }
bool Alu::get_n() { return f_->getAt(2); }
bool Alu::get_hcy() { return f_->getAt(1); }
bool Alu::get_cy() { return f_->getAt(0); }

void Alu::set_a(u8 n) { a_->set(n); }
void Alu::set_z(bool b) { f_->setAt(3, b); }
void Alu::set_n(bool b) { f_->setAt(2, b); }
void Alu::set_hcy(bool b) { f_->setAt(1, b); }
void Alu::set_cy(bool b) { f_->setAt(0, b); }

void Alu::add_n(u8 n) {
  u8 a = get_a();
  set_cy(static_cast<u16>(a) + n > 0xff);
  set_hcy((a & 0xf) + (n & 0xf) > 0xf);
  set_z(a + n == 0);
  set_n(false);
  set_a(a + n);
}

void Alu::addc_n(u8 n, bool carry) { return add_n(n + carry); }

void Alu::sub_n(u8 n) {
  u8 a = get_a();
  set_cy(a < n);
  set_hcy((a & 0xf) < (n & 0xf));
  set_z(a - n == 0);
  set_n(true);
  set_a(a - n);
}

void Alu::subc_n(u8 n, bool carry) { sub_n(n + carry); }

void Alu::and_n(u8 n) {
  u8 a = get_a();
  set_cy(false);
  set_hcy(true);
  set_z((a & n) == 0);
  set_n(false);
  set_a(a &= n);
}

void Alu::or_n(u8 n) {
  u8 a = get_a();
  set_cy(false);
  set_hcy(false);
  set_z((a | n) == 0);
  set_n(false);
  set_a(a |= n);
}

void Alu::xor_n(u8 n) {
  u8 a = get_a();
  set_cy(false);
  set_hcy(false);
  set_z((a ^ n) == 0);
  set_n(false);
  set_a(a ^= n);
}

void Alu::cp_n(u8 n) {
  u8 a = get_a();
  set_cy(a < n);
  set_hcy((a & 0xf) < (n & 0xf));
  set_z(a - n == 0);
  set_n(true);
}

void Alu::add_hl_n16(Register* r, u16 n) {
  u16 hl = r->get();
  set_cy(static_cast<u32>(hl) + n > 0xffff);
  set_hcy((hl & 0xfff) + (n & 0xfff) > 0xfff);
  set_n(false);
  r->set(hl + n);
}

void Alu::add_sp_n8(Register* r, u8 n) {
  u16 sp = r->get();
  set_cy((sp & 0xff) + n > 0xff);
  set_hcy((sp & 0xf) + (n & 0xf) > 0xf);
  set_z(false);
  set_n(false);
  r->set(sp + n);
}

void Alu::inc16(Register* r) { r->increment(); }

void Alu::dec16(Register* r) { r->decrement(); }

}  // namespace gbemu
