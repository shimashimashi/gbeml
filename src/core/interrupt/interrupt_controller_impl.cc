#include "core/interrupt/interrupt_controller_impl.h"

namespace gbemu {

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

void InterruptControllerImpl::clearVBlank() { interrupt_flag.setAt(0, false); }

void InterruptControllerImpl::clearLcdStat() { interrupt_flag.setAt(1, false); }

bool InterruptControllerImpl::isVBlankEnabled() {
  return interrupt_enable.getAt(0);
}

bool InterruptControllerImpl::isVBlankRequested() {
  return interrupt_flag.getAt(0);
}

bool InterruptControllerImpl::isLcdStatEnabled() {
  return interrupt_enable.getAt(1);
}

bool InterruptControllerImpl::isLcdStatRequested() {
  return interrupt_flag.getAt(1);
}

}  // namespace gbemu
