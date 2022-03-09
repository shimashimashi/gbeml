#ifndef GBEML_INTERRUPT_CONTROLLER_IMPL_H_
#define GBEML_INTERRUPT_CONTROLLER_IMPL_H_

#include "core/interrupt/interrupt_controller.h"
#include "core/register/register.h"
#include "core/types/types.h"

namespace gbeml {

class InterruptControllerImpl : public InterruptController {
 public:
  InterruptControllerImpl() : interrupt_flag(), interrupt_enable() {}
  InterruptControllerImpl(u8 interrupt_flag_, u8 interrupt_enable_)
      : interrupt_flag(interrupt_flag_), interrupt_enable(interrupt_enable_) {}

  virtual u8 readInterruptFlag() const override;
  virtual void writeInterruptFlag(u8 value) override;

  virtual u8 readInterruptEnable() const override;
  virtual void writeInterruptEnable(u8 value) override;

  virtual void signalVBlank() override;
  virtual void signalLcdStat() override;
  virtual void signalTimer() override;
  virtual void signalSerial() override;
  virtual void signalJoypad() override;

  virtual void clearVBlank() override;
  virtual void clearLcdStat() override;
  virtual void clearTimer() override;
  virtual void clearSerial() override;
  virtual void clearJoypad() override;

  virtual bool isVBlankRequested() override;
  virtual bool isLcdStatRequested() override;
  virtual bool isTimerRequested() override;
  virtual bool isSerialRequested() override;
  virtual bool isJoypadRequested() override;

  virtual bool isInterruptRequested() override;

 private:
  Register interrupt_flag;
  Register interrupt_enable;
};

}  // namespace gbeml

#endif  // GBEML_INTERRUPT_CONTROLLER_IMPL_H_
