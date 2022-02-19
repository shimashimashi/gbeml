#ifndef GBEMU_REGISTER_H_
#define GBEMU_REGISTER_H_

#include "types.h"

namespace gbemu {

class Register {
 public:
  Register() : value(0) {}
  Register(u8 value_) : value(value_) {}

  u8 get();
  bool getAt(u8 i);
  void set(u8 n);
  void setAt(u8 i, bool b);
  void increment();
  void decrement();

 private:
  u8 value;
};

class RegisterPair {
 public:
  RegisterPair(Register* high_, Register* low_) : high(high_), low(low_) {}

  u16 get();
  void set(u16 n);
  Register* getHigh();
  Register* getLow();
  void increment();
  void decrement();

 private:
  Register* high;
  Register* low;
};

}  // namespace gbemu

#endif  // GBEMU_REGISTER_H_
