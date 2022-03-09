#ifndef GBEML_CPU_H_
#define GBEML_CPU_H_

#include <string>

#include "core/bus/bus.h"
#include "core/cpu/alu.h"
#include "core/cpu/opcode.h"
#include "core/interrupt/interrupt_controller.h"
#include "core/register/register.h"
#include "core/types/types.h"

namespace gbeml {

class Cpu {
 public:
  Cpu(Bus* bus_, InterruptController* ic_)
      : bus(bus_), ic(ic_), af(), bc(), de(), hl(), pc(), sp(), alu(&af) {}

  u16 get_af();
  u16 get_bc();
  u16 get_de();
  u16 get_hl();
  u16 get_sp();
  u16 get_pc();

  u8 get_a();
  u8 get_f();
  u8 get_b();
  u8 get_c();
  u8 get_d();
  u8 get_e();
  u8 get_h();
  u8 get_l();

  bool get_carry();
  bool get_z();

  void set_af(u16 n);
  void set_bc(u16 n);
  void set_de(u16 n);
  void set_hl(u16 n);
  void set_sp(u16 n);
  void set_pc(u16 n);

  void set_a(u8 n);
  void set_f(u8 n);
  void set_b(u8 n);
  void set_c(u8 n);
  void set_d(u8 n);
  void set_e(u8 n);
  void set_h(u8 n);
  void set_l(u8 n);

  void set_carry(bool flag);
  void set_z(bool flag);

  void tick();
  void advance(u64 n);

  bool isStalled();
  bool isHalted();
  bool interruptEnabled();

  void setBreakpoint(i32 breakpoint);

 private:
  Bus* bus;
  InterruptController* ic;

  bool ime;
  u64 stalls = 0;
  bool halted = false;
  i64 breakpoint = -1;

  RegisterPair af;
  RegisterPair bc;
  RegisterPair de;
  RegisterPair hl;
  RegisterPair pc;
  RegisterPair sp;

  Alu alu;

  u8 fetch();
  u16 fetchWord();

  void execute(const Opcode& opcode);
  void execute_cb(const Opcode& opcode);

  void handleInterrupt();

  // 00xxx110
  void load_r_n8(const Opcode& opcode);
  // 01xxxyyy
  void load_r_r(const Opcode& opcode);
  // 00xx0010
  void load_r_a(const Opcode& opcode);
  // 00xx1010
  void load_a_r(const Opcode& opcode);
  // 11110010
  void load_a_c();
  // 11100010
  void load_c_a();
  // 11100000
  void load_n_a();
  // 11110000
  void load_a_n();
  // 00xx0001
  void load_r_n16(const Opcode& opcode);
  // 11111001
  void load_sp_hl();
  // 11111000
  void load_hl_sp_n8();
  // 00001000
  void load_n16_sp();
  // 11xx0101
  void push(const Opcode& opcode);
  // 11xx0001
  void pop(const Opcode& opcode);
  // 11101010
  void load_n16_a();
  // 11111010
  void load_a_n16();
  // 10000xxx
  void add_a_r(const Opcode& opcode);
  // 11000110
  void add_a_n();
  // 10001xxx
  void addc_a_r(const Opcode& opcode);
  // 11001110
  void addc_a_n();
  // 10010xxx
  void sub_a_r(const Opcode& opcode);
  // 11010110
  void sub_a_n();
  // 10011xxx
  void subc_a_r(const Opcode& opcode);
  // 11011110
  void subc_a_n();
  // 10100xxx
  void and_a_r(const Opcode& opcode);
  // 11100110
  void and_a_n();
  // 10110xxx
  void or_a_r(const Opcode& opcode);
  // 11110110
  void or_a_n();
  // 10101xxx
  void xor_a_r(const Opcode& opcode);
  // 11101110
  void xor_a_n();
  // 10111xxx
  void cp_a_r(const Opcode& opcode);
  // 11111110
  void cp_a_n();
  // 00xxx100
  void inc_r8(const Opcode& opcode);
  // 00xxx101
  void dec_r8(const Opcode& opcode);
  // 00xx1001
  void add_hl_r(const Opcode& opcode);
  // 11101000
  void add_sp_n();
  // 00xx0011
  void inc_r16(const Opcode& opcode);
  // 00xx1011
  void dec_r16(const Opcode& opcode);
  // CB + 00110xxx
  void swap(const Opcode& opcode);
  // 00100111
  void daa();
  // 00101111
  void cpl();
  // 00111111
  void ccf();
  // 00110111
  void scf();
  // 00000000
  void nop();
  // 01110110
  void halt();
  // 10 + 00000000
  void stop();
  // 11110011
  void di();
  // 11111011
  void ei();
  // 00000111
  void rlca();
  // 00010111
  void rla();
  // 00001111
  void rrca();
  // 00011111
  void rra();
  // CB + 00000xxx
  void rlc_n(const Opcode& opcode);
  // CB + 00010xxx
  void rl_n(const Opcode& opcode);
  // CB + 00001xxx
  void rrc_n(const Opcode& opcode);
  // CB + 00011xxx
  void rr_n(const Opcode& opcode);
  // CB + 00100xxx
  void sla_n(const Opcode& opcode);
  // CB + 00101xxx
  void sra_n(const Opcode& opcode);
  // CB + 00111xxx
  void srl_n(const Opcode& opcode);
  // CB + 01xxxyyy
  void bit_b_r(const Opcode& opcode);
  // CB + 11000xxx
  void set_b_r(const Opcode& opcode);
  // CB + 10000xxx
  void res_b_r(const Opcode& opcode);
  // 11000011
  void jp_n16();
  // 110xx010
  void jp_cc_n16(const Opcode& opcode);
  // 11101001
  void jp_hl();
  // 00011000
  void jr_n();
  // 001xx000
  void jr_cc_n(const Opcode& opcode);
  // 11001101
  void call_n16();
  // 110xx100
  void call_cc_n16(const Opcode& opcode);
  // 11xxx111
  void rst_n(const Opcode& opcode);
  // 11001001
  void ret();
  // 110xx000
  void ret_cc(const Opcode& opcode);
  // 11011001
  void reti();

  u8 readRegister(u8 r);
  void writeRegister(u8 r, u8 n);

  Register* selectRegister(u8 r);
  RegisterPair* selectRegisterPair(u8 r);

  u8 readMemory(u16 addr);
  void writeMemory(u16 addr, u8 value);

  u16 readWord(u16 addr);
  void writeWord(u16 addr, u16 value);

  void jumpRelative(u8 offset);

  bool checkFlags(u8 n);

  void pushStack(u16 word);
  u16 popStack();

  void call(u16 addr);
};

}  // namespace gbeml

#endif  // GBEML_CPU_H_
