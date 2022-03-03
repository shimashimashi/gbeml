#ifndef GBEMU_INTERRUPT_CONTROLLER_IMPL_H_
#define GBEMU_INTERRUPT_CONTROLLER_IMPL_H_

#include "core/interrupt/interrupt_controller.h"
#include "core/register/register.h"
#include "core/types/types.h"

namespace gbemu {

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

  virtual void clearVBlank() override;
  virtual void clearLcdStat() override;

  virtual bool isVBlankEnabled() override;
  virtual bool isLcdStatEnabled() override;

  virtual bool isVBlankRequested() override;
  virtual bool isLcdStatRequested() override;

 private:
  Register interrupt_flag;
  Register interrupt_enable;
};

}  // namespace gbemu

#endif  // GBEMU_INTERRUPT_CONTROLLER_IMPL_H_
