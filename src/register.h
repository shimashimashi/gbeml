#ifndef GBEMU_REGISTER_H_
#define GBEMU_REGISTER_H_

#include "types.h"

namespace gbemu {

class HalfRegister {
 public:
  HalfRegister() : value(0) {}

  u8 get();
  bool getAt(u8 i);
  void set(u8 n);
  void setAt(u8 i, bool b);
  void increment();
  void decrement();

 private:
  u8 value;
};

class Register {
 public:
  Register(HalfRegister* high_, HalfRegister* low_) : high(high_), low(low_) {}

  u16 get();
  void set(u16 n);
  void increment();
  void decrement();

 private:
  HalfRegister* high;
  HalfRegister* low;
};

}  // namespace gbemu

#endif  // GBEMU_REGISTER_H_
