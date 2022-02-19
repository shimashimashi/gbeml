#ifndef GBEMU_ALU_H_
#define GBEMU_ALU_H_

#include "bus.h"
#include "cpu/register.h"
#include "types.h"

namespace gbemu {

class Alu {
 public:
  Alu(RegisterPair* af_) : af(af_) {}

  void add_n(u8 n);
  void addc_n(u8 n, bool carry);
  void sub_n(u8 n);
  void subc_n(u8 n, bool carry);
  void and_n(u8 n);
  void or_n(u8 n);
  void xor_n(u8 n);
  void cp_n(u8 n);
  void add_hl_n16(RegisterPair* r, u16 n);
  void add_sp_n8(RegisterPair* r, u8 n);

  void inc_r(Register* r);
  void dec_r(Register* r);
  void inc_memory(u16 addr, Bus* bus);
  void dec_memory(u16 addr, Bus* bus);

  void swap_r(Register* r);
  void swap_memory(u16 addr, Bus* bus);

  void daa();

  u8 get_a();
  bool get_z();
  bool get_n();
  bool get_h();
  bool get_c();

  void set_a(u8 n);
  void set_z(bool b);
  void set_n(bool b);
  void set_h(bool b);
  void set_c(bool b);

 private:
  RegisterPair* af;

  u8 swap(u8 n);

  void setFlagsInc(u8 n);
  void setFlagsDec(u8 n);
  void setFlagsSwap(u8 n);
};

}  // namespace gbemu

#endif  // GBEMU_ALU_H_
