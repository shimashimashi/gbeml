#ifndef GBEML_LCD_STAT_H_
#define GBEML_LCD_STAT_H_

#include "core/register/register.h"
#include "core/types/types.h"

namespace gbeml {

class LcdStat {
 public:
  LcdStat(u8 flags_) : flags(flags_) {}

  u8 read() const;
  void write(u8 value);

  bool isLycEqualsLyInterruptEnabled() const;
  bool isOamScanInterruptEnabled() const;
  bool isVBlankInterruptEnabled() const;
  bool isHBlankInterruptEnabled() const;

 private:
  Register flags;
};

}  // namespace gbeml

#endif  // GBEML_LCD_STAT_H_
