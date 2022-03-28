#include "core/timer/timer_impl.h"

#include "core/log/logging.h"

namespace gbeml {

void TimerImpl::tick() {
  tickDivider();
  tickCounter();
}

void TimerImpl::tickDivider() {
  if (++div_cycles % 256 == 0) {
    div.increment();
  }
}

void TimerImpl::tickCounter() {
  if (!tac.getAt(2)) {
    return;
  }

  tima_cycles++;
  switch (tac.slice(0, 1)) {
    case 0:
      if (tima_cycles % 1024 == 0) {
        tima.increment();
      }
      break;
    case 1:
      if (tima_cycles % 16 == 0) {
        tima.increment();
      }
      break;
    case 2:
      if (tima_cycles % 64 == 0) {
        tima.increment();
      }
      break;
    case 3:
      if (tima_cycles % 256 == 0) {
        tima.increment();
      }
      break;
    default:
      DCHECK(false);
      break;
  }

  if (tima.get() == 0) {
    tima.set(tma.get());
    ic->signalTimer();
  }
}

u8 TimerImpl::readDivider() const { return div.get(); }

u8 TimerImpl::readCounter() const { return tima.get(); }

u8 TimerImpl::readModulo() const { return tma.get(); }

u8 TimerImpl::readControl() const { return tac.get(); }

void TimerImpl::resetDivider() { div.set(0); }

void TimerImpl::writeCounter(u8 value) { tima.set(value); }

void TimerImpl::writeModulo(u8 value) { tma.set(value); }

void TimerImpl::writeControl(u8 value) { tac.set(value); }

}  // namespace gbeml
