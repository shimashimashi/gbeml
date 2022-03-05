#ifndef GBEML_INTERRUPT_CONTROLLER_H_
#define GBEML_INTERRUPT_CONTROLLER_H_

#include "core/register/register.h"
#include "core/types/types.h"

namespace gbeml {

class InterruptController {
 public:
  virtual ~InterruptController();

  virtual u8 readInterruptFlag() const = 0;
  virtual void writeInterruptFlag(u8 value) = 0;

  virtual u8 readInterruptEnable() const = 0;
  virtual void writeInterruptEnable(u8 value) = 0;

  virtual void signalVBlank() = 0;
  virtual void signalLcdStat() = 0;

  virtual void clearVBlank() = 0;
  virtual void clearLcdStat() = 0;

  virtual bool isVBlankEnabled() = 0;
  virtual bool isLcdStatEnabled() = 0;

  virtual bool isVBlankRequested() = 0;
  virtual bool isLcdStatRequested() = 0;
};

}  // namespace gbeml

#endif  // GBEML_INTERRUPT_CONTROLLER_H_
