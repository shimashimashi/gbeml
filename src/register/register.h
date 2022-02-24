#ifndef GBEMU_REGISTER_H_
#define GBEMU_REGISTER_H_

#include "types/types.h"

namespace gbemu {

class Register {
 public:
  Register() : value(0) {}
  Register(u8 value_) : value(value_) {}

  u8 get() const;
  bool getAt(u8 i) const;
  u8 slice(u8 from, u8 to) const;

  void set(u8 n);
  void setAt(u8 i, bool b);
  void increment();
  void decrement();

 private:
  u8 value;
};

class RegisterPair {
 public:
  RegisterPair() : high(0), low(0) {}
  RegisterPair(u8 high_, u8 low_) : high(high_), low(low_) {}

  u16 get() const;
  Register* getHigh();
  Register* getLow();

  void set(u16 n);
  void increment();
  void decrement();

 private:
  Register high;
  Register low;
};

}  // namespace gbemu

#endif  // GBEMU_REGISTER_H_
