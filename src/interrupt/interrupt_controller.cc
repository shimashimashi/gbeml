#include "interrupt/interrupt_controller.h"

namespace gbemu {

u8 InterruptController::readIf() const { return if_register.get(); }

void InterruptController::writeIf(u8 value) { if_register.set(value); }

u8 InterruptController::readIe() const { return ie_register.get(); }

void InterruptController::writeIe(u8 value) { ie_register.set(value); }

void InterruptController::signalVBlank() { if_register.setAt(0, true); }

void InterruptController::signalLcdStat() { if_register.setAt(1, true); }

void InterruptController::clearVBlank() { if_register.setAt(0, false); }

void InterruptController::clearLcdStat() { if_register.setAt(1, false); }

bool InterruptController::isVBlankEnabled() { return ie_register.getAt(0); }

bool InterruptController::isVBlankRequested() { return if_register.getAt(0); }

bool InterruptController::isLcdStatEnabled() { return ie_register.getAt(1); }

bool InterruptController::isLcdStatRequested() { return if_register.getAt(1); }

}  // namespace gbemu
