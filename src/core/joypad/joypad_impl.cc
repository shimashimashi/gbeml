#include "core/joypad/joypad_impl.h"

namespace gbeml {

u8 JoypadImpl::read() const {
  switch (mode) {
    case JoypadMode::Action:
      return 0b11010000 | (action.get() & 0x0f);
    case JoypadMode::Direction:
      return 0b11100000 | (direction.get() & 0x0f);
  }
}

void JoypadImpl::write(u8 value) {
  if ((value & 0b00100000) == 0) {
    mode = JoypadMode::Action;
  } else {
    mode = JoypadMode::Direction;
  }
}

void JoypadImpl::press(JoypadButton button) {
  switch (button) {
    case JoypadButton::Start:
      action.setAt(3, false);
      break;
    case JoypadButton::Select:
      action.setAt(2, false);
      break;
    case JoypadButton::B:
      action.setAt(1, false);
      break;
    case JoypadButton::A:
      action.setAt(0, false);
      break;
    case JoypadButton::Down:
      direction.setAt(3, false);
      break;
    case JoypadButton::Up:
      direction.setAt(2, false);
      break;
    case JoypadButton::Left:
      direction.setAt(1, false);
      break;
    case JoypadButton::Right:
      direction.setAt(0, false);
      break;
  }

  ic->signalJoypad();
}

void JoypadImpl::release(JoypadButton button) {
  switch (button) {
    case JoypadButton::Start:
      action.setAt(3, true);
      break;
    case JoypadButton::Select:
      action.setAt(2, true);
      break;
    case JoypadButton::B:
      action.setAt(1, true);
      break;
    case JoypadButton::A:
      action.setAt(0, true);
      break;
    case JoypadButton::Down:
      direction.setAt(3, true);
      break;
    case JoypadButton::Up:
      direction.setAt(2, true);
      break;
    case JoypadButton::Left:
      direction.setAt(1, true);
      break;
    case JoypadButton::Right:
      direction.setAt(0, true);
      break;
  }
}

}  // namespace gbeml
