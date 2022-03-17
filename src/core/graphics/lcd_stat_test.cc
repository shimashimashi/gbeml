#include "core/graphics/lcd_stat.h"

#include <gtest/gtest.h>

namespace gbeml {

TEST(LcdStatTest, read) {
  LcdStat lcd_stat(0);

  lcd_stat.write(0xff);
  EXPECT_EQ(0b11111000, lcd_stat.read());
}

TEST(LcdStatTest, isInterruptEnabled) {
  LcdStat lcd_stat(0b01001000);
  EXPECT_EQ(true, lcd_stat.isHBlankInterruptEnabled());
  EXPECT_EQ(false, lcd_stat.isVBlankInterruptEnabled());
  EXPECT_EQ(false, lcd_stat.isOamScanInterruptEnabled());
  EXPECT_EQ(true, lcd_stat.isLycEqualsLyInterruptEnabled());
}

}  // namespace gbeml
