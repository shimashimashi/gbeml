#ifndef GBEMU_ALU_H_
#define GBEMU_ALU_H_

#include "core/bus/bus.h"
#include "core/register/register.h"
#include "core/types/types.h"

namespace gbemu {

class Alu {
 public:
  Alu(RegisterPair* af_) : af(af_) {}

  void add_n(u8 n);
  void addc_n(u8 n);
  void sub_n(u8 n);
  void subc_n(u8 n);
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

  void cpl();
  void ccf();
  void scf();

  void rlca();
  void rla();
  void rrca();
  void rra();
  void rlc_r(Register* r);
  void rlc_memory(u16 addr, Bus* bus);
  void rl_r(Register* r);
  void rl_memory(u16 addr, Bus* bus);
  void rrc_r(Register* r);
  void rrc_memory(u16 addr, Bus* bus);
  void rr_r(Register* r);
  void rr_memory(u16 addr, Bus* bus);

  void sla_r(Register* r);
  void sla_memory(u16 addr, Bus* bus);
  void sra_r(Register* r);
  void sra_memory(u16 addr, Bus* bus);
  void srl_r(Register* r);
  void srl_memory(u16 addr, Bus* bus);

  void bit_b_r(u8 i, u8 n);
  void set_b_r(u8 i, Register* r);
  void set_b_memory(u8 i, u16 addr, Bus* bus);
  void res_b_r(u8 i, Register* r);
  void res_b_memory(u8 i, u16 addr, Bus* bus);

  u8 get_a();
  bool get_z();
  bool get_n();
  bool get_h();
  bool get_c();

  void set_a(u8 n);
  void set_f(u8 n);
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

  u8 rotateLeft(u8 n);
  u8 rotateLeftThroughCarry(u8 n);
  u8 rotateRight(u8 n);
  u8 rotateRightThroughCarry(u8 n);

  u8 shiftLeft(u8 n);
  u8 shiftRightArithmetic(u8 n);
  u8 shiftRightLogical(u8 n);

  void testBit(u8 n, u8 i);
  u8 setBit(u8 n, u8 i);
  u8 resetBit(u8 n, u8 i);
};

}  // namespace gbemu

#endif  // GBEMU_ALU_H_
