#ifndef GBEML_JOYPAD_IMPL_H_
#define GBEML_JOYPAD_IMPL_H_

#include "core/joypad/joypad.h"

namespace gbeml {

class JoypadImpl : public Joypad {
 public:
  JoypadImpl(InterruptController* ic_)
      : ic(ic_), action(0xff), direction(0xff), mode(JoypadMode::Action){};

  u8 read() const;
  void write(u8 value);

  void press(JoypadButton button);
  void release(JoypadButton button);

 private:
  InterruptController* ic;
  Register action;
  Register direction;
  JoypadMode mode;
};

}  // namespace gbeml

#endif  // GBEML_JOYPAD_IMPL_H_
