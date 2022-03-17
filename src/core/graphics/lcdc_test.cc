#include "core/graphics/lcdc.h"

#include <gtest/gtest.h>

namespace gbeml {

TEST(LcdcTest, getBackgroundTileDataAddress) {
  Lcdc lcdc(0);

  lcdc.write(0b00010000);
  EXPECT_EQ(0b00010000, lcdc.read());

  EXPECT_EQ(0x0000, lcdc.getBackgroundTileDataAddress(0x00));
  EXPECT_EQ(0x0010, lcdc.getBackgroundTileDataAddress(0x01));
  EXPECT_EQ(0x0ff0, lcdc.getBackgroundTileDataAddress(0xff));

  lcdc.write(0b00000000);
  EXPECT_EQ(0x1000, lcdc.getBackgroundTileDataAddress(0x00));
  EXPECT_EQ(0x1010, lcdc.getBackgroundTileDataAddress(0x01));
  EXPECT_EQ(0x17f0, lcdc.getBackgroundTileDataAddress(0x7f));
  EXPECT_EQ(0x0800, lcdc.getBackgroundTileDataAddress(0x80));
  EXPECT_EQ(0x0810, lcdc.getBackgroundTileDataAddress(0x81));
  EXPECT_EQ(0x0ff0, lcdc.getBackgroundTileDataAddress(0xff));
}

TEST(LcdcTest, getBackgroundTileMapAddress) {
  Lcdc lcdc(0);

  lcdc.write(0b00000000);
  EXPECT_EQ(0x1800, lcdc.getBackgroundTileMapAddress(0x0000));
  EXPECT_EQ(0x1801, lcdc.getBackgroundTileMapAddress(0x0001));
  EXPECT_EQ(0x1bff, lcdc.getBackgroundTileMapAddress(0x03ff));

  lcdc.write(0b00001000);
  EXPECT_EQ(0x1c00, lcdc.getBackgroundTileMapAddress(0x0000));
  EXPECT_EQ(0x1c01, lcdc.getBackgroundTileMapAddress(0x0001));
  EXPECT_EQ(0x1fff, lcdc.getBackgroundTileMapAddress(0x03ff));
}

TEST(LcdcTest, getWindowTileMapAddress) {
  Lcdc lcdc(0);

  lcdc.write(0b00000000);
  EXPECT_EQ(0x1800, lcdc.getWindowTileMapAddress(0x0000));
  EXPECT_EQ(0x1801, lcdc.getWindowTileMapAddress(0x0001));
  EXPECT_EQ(0x1bff, lcdc.getWindowTileMapAddress(0x03ff));

  lcdc.write(0b01000000);
  EXPECT_EQ(0x1c00, lcdc.getWindowTileMapAddress(0x0000));
  EXPECT_EQ(0x1c01, lcdc.getWindowTileMapAddress(0x0001));
  EXPECT_EQ(0x1fff, lcdc.getWindowTileMapAddress(0x03ff));
}

}  // namespace gbeml
