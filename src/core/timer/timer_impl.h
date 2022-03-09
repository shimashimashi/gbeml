#ifndef GBEML_TIMER_IMPL_H_
#define GBEML_TIMER_IMPL_H_

#include "core/interrupt/interrupt_controller.h"
#include "core/timer/timer.h"

namespace gbeml {

class TimerImpl : public Timer {
 public:
  TimerImpl(InterruptController* ic_, u8 div_, u8 tima_, u8 tma_, u8 tac_)
      : ic(ic_), div(div_), tima(tima_), tma(tma_), tac(tac_) {}

  virtual void tick() override;

  virtual u8 readDivider() const override;
  virtual u8 readCounter() const override;
  virtual u8 readModulo() const override;
  virtual u8 readControl() const override;

  virtual void resetDivider() override;
  virtual void writeCounter(u8 value) override;
  virtual void writeModulo(u8 value) override;
  virtual void writeControl(u8 value) override;

  void tickDivider();
  void tickCounter();

 private:
  InterruptController* ic;
  Register div;
  Register tima;
  Register tma;
  Register tac;

  u64 div_cycles = 0;
  u64 tima_cycles = 0;
};

}  // namespace gbeml

#endif  // GBEML_TIMER_IMPL_H_
