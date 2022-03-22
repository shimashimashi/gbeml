#ifndef GBEML_JOYPAD_H_
#define GBEML_JOYPAD_H_

#include "core/interrupt/interrupt_controller.h"
#include "core/register/register.h"
#include "core/types/types.h"

namespace gbeml {

enum class JoypadButton { Select, Start, A, B, Down, Up, Left, Right };

enum class JoypadMode { Action, Direction };

class Joypad {
 public:
  virtual ~Joypad();

  virtual u8 read() const = 0;
  virtual void write(u8 value) = 0;

  virtual void press(JoypadButton button) = 0;
  virtual void release(JoypadButton button) = 0;
};

}  // namespace gbeml

#endif  // GBEML_JOYPAD_H_
