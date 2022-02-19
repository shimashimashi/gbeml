#ifndef GBEMU_CPU_H_
#define GBEMU_CPU_H_

#include <string>

#include "bus.h"
#include "cpu/alu.h"
#include "cpu/opcode.h"
#include "cpu/register.h"
#include "types.h"

namespace gbemu {

class Cpu {
 public:
  Cpu(Bus* bus_) : bus(bus_) {
    a = new Register();
    f = new Register();
    b = new Register();
    c = new Register();
    d = new Register();
    e = new Register();
    h = new Register();
    l = new Register();
    pc1 = new Register();
    pc2 = new Register();
    sp1 = new Register();
    sp2 = new Register();

    af = new RegisterPair(a, f);
    bc = new RegisterPair(b, c);
    de = new RegisterPair(d, e);
    hl = new RegisterPair(h, l);
    pc = new RegisterPair(pc1, pc2);
    sp = new RegisterPair(sp1, sp2);

    alu = new Alu(af);
  }

  void tick();

 private:
  Bus* bus;
  Alu* alu;

  RegisterPair* af;
  RegisterPair* bc;
  RegisterPair* de;
  RegisterPair* hl;
  RegisterPair* pc;
  RegisterPair* sp;

  Register* a;
  Register* f;
  Register* b;
  Register* c;
  Register* d;
  Register* e;
  Register* h;
  Register* l;
  Register* pc1;
  Register* pc2;
  Register* sp1;
  Register* sp2;

  u16 get_af();
  u16 get_bc();
  u16 get_de();
  u16 get_hl();
  u16 get_sp();

  void set_af(u16 n);
  void set_bc(u16 n);
  void set_de(u16 n);
  void set_hl(u16 n);
  void set_sp(u16 n);

  u8 get_a();
  u8 get_f();
  u8 get_b();
  u8 get_c();
  u8 get_d();
  u8 get_e();
  u8 get_h();
  u8 get_l();

  void set_a(u8 n);
  void set_f(u8 n);
  void set_b(u8 n);
  void set_c(u8 n);
  void set_d(u8 n);
  void set_e(u8 n);
  void set_h(u8 n);
  void set_l(u8 n);

  u8 fetch();
  u16 fetchWord();

  void execute(const Opcode& opcode);
  void execute_cb(const Opcode& opcode);

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
  void jp_cc_n16();
  // 11101001
  void jp_hl();
  // 00011000
  void jr_n();
  // 001xx000
  void jr_cc_n();
  // 11001101
  void call_n16();
  // 110xx100
  void call_cc_n16();
  // 11xxx111
  void rst_n();
  // 11001001
  void ret();
  // 110xx000
  void ret_cc();
  // 11011001
  void reti();

  u8 readRegister(u8 r);
  void writeRegister(u8 r, u8 n);

  Register* selectRegister(u8 r);

  u8 readMemory(u16 addr);
  void writeMemory(u16 addr, u8 value);

  u16 readWord(u16 addr);
  void writeWord(u16 addr, u16 value);

  u64 stalls = 0;
};

}  // namespace gbemu

#endif  // GBEMU_CPU_H_
