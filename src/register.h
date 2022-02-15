#ifndef GBEMU_REGISTER_H_
#define GBEMU_REGISTER_H_

#include "types.h"

namespace gbemu {

class Register {
 public:
  Register() : value(0) {}

  u16 get();
  u8 getLow();
  u8 getHigh();
  bool getAt(u8 i);

  void set(u16 n);
  void setLow(u8 n);
  void setHigh(u8 n);
  void setAt(u8 i, bool b);
  void increment();
  void decrement();

 private:
  u16 value;
};

}  // namespace gbemu

#endif  // GBEMU_REGISTER_H_
