#ifndef GBEMU_ALU_H_
#define GBEMU_ALU_H_

#include "register.h"
#include "types.h"

namespace gbemu {

class Alu {
 public:
  Alu(HalfRegister* a, HalfRegister* f) : a_(a), f_(f) {}

  void add_n(u8 n);
  void addc_n(u8 n, bool carry);
  void sub_n(u8 n);
  void subc_n(u8 n, bool carry);
  void and_n(u8 n);
  void or_n(u8 n);
  void xor_n(u8 n);
  void cp_n(u8 n);

  void add_hl_n16(Register* r, u16 n);
  void add_sp_n8(Register* r, u8 n);
  void inc16(Register* r);
  void dec16(Register* r);

 private:
  HalfRegister* a_;
  HalfRegister* f_;

  u8 get_a();
  bool get_z();
  bool get_n();
  bool get_hcy();
  bool get_cy();

  void set_a(u8 n);
  void set_z(bool b);
  void set_n(bool b);
  void set_hcy(bool b);
  void set_cy(bool b);
};

}  // namespace gbemu

#endif  // GBEMU_ALU_H_
