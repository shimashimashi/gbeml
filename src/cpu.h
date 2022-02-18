#ifndef GBEMU_CPU_H_
#define GBEMU_CPU_H_

#include <string>

#include "alu.h"
#include "bus.h"
#include "opcode.h"
#include "register.h"
#include "types.h"

namespace gbemu {

class Cpu {
 public:
  Cpu(Bus* bus_) : bus(bus_) {
    a = new HalfRegister();
    f = new HalfRegister();
    b = new HalfRegister();
    c = new HalfRegister();
    d = new HalfRegister();
    e = new HalfRegister();
    h = new HalfRegister();
    l = new HalfRegister();
    pc1 = new HalfRegister();
    pc2 = new HalfRegister();
    sp1 = new HalfRegister();
    sp2 = new HalfRegister();

    af = new Register(a, f);
    bc = new Register(b, c);
    de = new Register(d, e);
    hl = new Register(h, l);
    pc = new Register(pc1, pc2);
    sp = new Register(sp1, sp2);

    alu = new Alu(a, f);
  }

  void tick();

 private:
  Bus* bus;
  Alu* alu;

  Register* af;
  Register* bc;
  Register* de;
  Register* hl;
  Register* pc;
  Register* sp;

  HalfRegister* a;
  HalfRegister* f;
  HalfRegister* b;
  HalfRegister* c;
  HalfRegister* d;
  HalfRegister* e;
  HalfRegister* h;
  HalfRegister* l;
  HalfRegister* pc1;
  HalfRegister* pc2;
  HalfRegister* sp1;
  HalfRegister* sp2;

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

  void set_z(bool b);
  void set_n(bool b);
  void set_hcy(bool b);
  void set_cy(bool b);

  u8 fetch();
  u16 fetchWord();

  void execute(const Opcode& opcode);

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
  // 10001xxx
  void addc_a_r(const Opcode& opcode);
  // 10010xxx
  void sub_a_r(const Opcode& opcode);
  // 10011xxx
  void subc_a_r(const Opcode& opcode);
  // 10100xxx
  void and_a_r(const Opcode& opcode);
  // 10110xxx
  void or_a_r(const Opcode& opcode);
  // 10101xxx
  void xor_a_r(const Opcode& opcode);
  // 10111xxx
  void cp_a_r(const Opcode& opcode);
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
  // 01110110
  void halt();

  u8 readHalfRegister(u8 r);
  void writeHalfRegister(u8 r, u8 n);

  u8 readMemory(u16 addr);
  void writeMemory(u16 addr, u8 value);

  u16 readWord(u16 addr);
  void writeWord(u16 addr, u16 value);

  u64 stalls = 0;
};

}  // namespace gbemu

#endif  // GBEMU_CPU_H_
