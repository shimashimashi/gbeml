#include "core/graphics/tile.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "core/graphics/color.h"
#include "core/graphics/palette.h"

namespace gbeml {

TEST(TileTest, popPixel) {
  BackgroundPalette palette(0b11100100);
  Tile tile(0b01010101, 0b00110011, palette);

  EXPECT_EQ(Color::White, tile.getAt(0));
  EXPECT_EQ(Color::LightGray, tile.getAt(1));
  EXPECT_EQ(Color::DarkGray, tile.getAt(2));
  EXPECT_EQ(Color::Black, tile.getAt(3));
}

}  // namespace gbeml
