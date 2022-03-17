#include "core/graphics/lcd_stat.h"

namespace gbeml {

u8 LcdStat::read() const {
  // clear lower 3 bits to be set later.
  return flags.get() & 0b11111000;
}

void LcdStat::write(u8 value) { return flags.set(value); }

bool LcdStat::isLycEqualsLyInterruptEnabled() const { return flags.getAt(6); }

bool LcdStat::isOamScanInterruptEnabled() const { return flags.getAt(5); }

bool LcdStat::isVBlankInterruptEnabled() const { return flags.getAt(4); }

bool LcdStat::isHBlankInterruptEnabled() const { return flags.getAt(3); }

}  // namespace gbeml
