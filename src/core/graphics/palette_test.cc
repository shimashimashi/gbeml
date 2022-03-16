#include "core/graphics/palette.h"

#include <gtest/gtest.h>

#include "core/graphics/color.h"

namespace gbeml {

TEST(PaletteTest, BackgroundPalette) {
  BackgroundPalette palette(0);
  palette.write(0b11100100);

  EXPECT_EQ(0b11100100, palette.read());
  EXPECT_EQ(Color::White, palette.getColor(0));
  EXPECT_EQ(Color::LightGray, palette.getColor(1));
  EXPECT_EQ(Color::DarkGray, palette.getColor(2));
  EXPECT_EQ(Color::Black, palette.getColor(3));
}

TEST(PaletteTest, SpritePalette) {
  SpritePalette palette(0);
  palette.write(0b10010000);

  EXPECT_EQ(0b10010000, palette.read());
  EXPECT_EQ(Color::Transparent, palette.getColor(0));
  EXPECT_EQ(Color::White, palette.getColor(1));
  EXPECT_EQ(Color::LightGray, palette.getColor(2));
  EXPECT_EQ(Color::DarkGray, palette.getColor(3));
}

}  // namespace gbeml
