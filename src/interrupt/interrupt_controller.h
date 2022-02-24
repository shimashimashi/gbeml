#ifndef GBEMU_INTERRUPT_CONTROLLER_H_
#define GBEMU_INTERRUPT_CONTROLLER_H_

#include "register/register.h"
#include "types/types.h"

namespace gbemu {

class InterruptController {
 public:
  InterruptController() : if_register(), ie_register() {}
  InterruptController(u8 if_, u8 ie_) : if_register(if_), ie_register(ie_) {}

  u8 readIf() const;
  void writeIf(u8 value);

  u8 readIe() const;
  void writeIe(u8 value);

  void signalVBlank();
  void signalLcdStat();

  void clearVBlank();
  void clearLcdStat();

  bool isVBlankEnabled();
  bool isLcdStatEnabled();

  bool isVBlankRequested();
  bool isLcdStatRequested();

 private:
  Register if_register;
  Register ie_register;
};

}  // namespace gbemu

#endif  // GBEMU_INTERRUPT_CONTROLLER_H_
