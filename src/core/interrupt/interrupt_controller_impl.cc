#include "core/interrupt/interrupt_controller_impl.h"

namespace gbeml {

u8 InterruptControllerImpl::readInterruptFlag() const {
  return interrupt_flag.get();
}

void InterruptControllerImpl::writeInterruptFlag(u8 value) {
  interrupt_flag.set(value);
}

u8 InterruptControllerImpl::readInterruptEnable() const {
  return interrupt_enable.get();
}

void InterruptControllerImpl::writeInterruptEnable(u8 value) {
  interrupt_enable.set(value);
}

void InterruptControllerImpl::signalVBlank() { interrupt_flag.setAt(0, true); }

void InterruptControllerImpl::signalLcdStat() { interrupt_flag.setAt(1, true); }

void InterruptControllerImpl::signalTimer() { interrupt_flag.setAt(2, true); }

void InterruptControllerImpl::signalSerial() { interrupt_flag.setAt(3, true); }

void InterruptControllerImpl::signalJoypad() { interrupt_flag.setAt(4, true); }

void InterruptControllerImpl::clearVBlank() { interrupt_flag.setAt(0, false); }

void InterruptControllerImpl::clearLcdStat() { interrupt_flag.setAt(1, false); }

void InterruptControllerImpl::clearTimer() { interrupt_flag.setAt(2, false); }

void InterruptControllerImpl::clearSerial() { interrupt_flag.setAt(3, false); }

void InterruptControllerImpl::clearJoypad() { interrupt_flag.setAt(4, false); }

bool InterruptControllerImpl::isVBlankRequested() {
  return interrupt_flag.getAt(0) && interrupt_enable.getAt(0);
}

bool InterruptControllerImpl::isLcdStatRequested() {
  return interrupt_flag.getAt(1) && interrupt_enable.getAt(1);
}

bool InterruptControllerImpl::isTimerRequested() {
  return interrupt_flag.getAt(2) && interrupt_enable.getAt(2);
}

bool InterruptControllerImpl::isSerialRequested() {
  return interrupt_flag.getAt(3) && interrupt_enable.getAt(3);
}

bool InterruptControllerImpl::isJoypadRequested() {
  return interrupt_flag.getAt(4) && interrupt_enable.getAt(4);
}

bool InterruptControllerImpl::isInterruptRequested() {
  return isVBlankRequested() || isLcdStatRequested() || isTimerRequested() ||
         isSerialRequested() || isJoypadRequested();
}

}  // namespace gbeml
