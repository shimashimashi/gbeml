#ifndef GBEML_TIMER_H_
#define GBEML_TIMER_H_

#include "core/register/register.h"
#include "core/types/types.h"

namespace gbeml {

class Timer {
 public:
  virtual ~Timer();

  virtual void tick() = 0;

  virtual u8 readDivider() const = 0;
  virtual u8 readCounter() const = 0;
  virtual u8 readModulo() const = 0;
  virtual u8 readControl() const = 0;

  virtual void resetDivider() = 0;
  virtual void writeCounter(u8 value) = 0;
  virtual void writeModulo(u8 value) = 0;
  virtual void writeControl(u8 value) = 0;
};

}  // namespace gbeml

#endif  // GBEML_TIMER_H_
