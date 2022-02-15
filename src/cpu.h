#ifndef GBEMU_CPU_H_
#define GBEMU_CPU_H_

#include <string>

#include "bus.h"
#include "register.h"
#include "types.h"

namespace gbemu {

class Cpu {
 public:
  Cpu(Bus* bus) : bus(bus) {
    Register* af = new Register();
    Register* bc = new Register();
    Register* de = new Register();
    Register* hl = new Register();
    Register* pc = new Register();
    Register* sp = new Register();
  };

 private:
  Bus* bus;
  u64 stalls = 0;

  Register* af;
  Register* bc;
  Register* de;
  Register* hl;
  Register* pc;
  Register* sp;

  u16 get_af();
  u16 get_bc();
  u16 get_de();
  u16 get_hl();

  void set_af(u16 n);
  void set_bc(u16 n);
  void set_de(u16 n);
  void set_hl(u16 n);

  u8 get_a();
  u8 get_f();
  u8 get_b();
  u8 get_c();
  u8 get_d();
  u8 get_e();
  u8 get_h();
  u8 get_l();

  bool get_z();
  bool get_n();
  bool get_hcy();
  bool get_cy();

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
  void execute(u8 opcode);

  // 00xxx110
  void load_r_n8(u8 opcode);

  // 01xxxyyy
  void load_r_r(u8 opcode);

  // 00xx0010
  void load_r_a(u8 opcode);

  // 00xx1010
  void load_a_r(u8 opcode);

  // 11110010
  void load_a_c(u8 opcode);

  // 11100010
  void load_c_a(u8 opcode);

  // 11100000
  void load_n_a(u8 opcode);

  // 11110000
  void load_a_n(u8 opcode);

  // 00xx0001
  void load_r_n16(u8 opcode);

  // 11111001
  void load_sp_hl(u8 opcode);

  // 11111000
  void load_hl_sp_n8(u8 opcode);

  // 00001000
  void load_n16_sp(u8 opcode);

  // 11xx0101
  void push(u8 opcode);

  // 11xx0001
  void pop(u8 opcode);

  // 11101010
  void load_n16_a(u8 opcode);

  // 11111010
  void load_a_n16(u8 opcode);

  // 01110110
  void halt(u8 opcode);

  bool match(u8 opcode, const std::string& pattern);

  u16 getRegister(u8 r);
  void setRegister(u8 r, u16 n);

  u8 readMemory(u16 addr);
  void writeMemory(u16 addr, u8 value);

  bool isCarrySetAdd8(u8 n1, u8 n2);
  bool isCarrySetAdd4(u8 n1, u8 n2);

  u16 concat(u8 n1, u8 n2);
};

}  // namespace gbemu

#endif  // GBEMU_CPU_H_
