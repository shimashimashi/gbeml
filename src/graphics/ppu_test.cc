#include "graphics/ppu.h"

#include <gtest/gtest.h>

namespace gbemu {

TEST(LcdcTest, bgTileDataAddress) {
  Lcdc lcdc(0);

  lcdc.write(0b00010000);
  EXPECT_EQ(0x0000, lcdc.bgTileDataAddress(0x00));
  EXPECT_EQ(0x0010, lcdc.bgTileDataAddress(0x01));
  EXPECT_EQ(0x0ff0, lcdc.bgTileDataAddress(0xff));

  lcdc.write(0b00000000);
  EXPECT_EQ(0x1000, lcdc.bgTileDataAddress(0x00));
  EXPECT_EQ(0x1010, lcdc.bgTileDataAddress(0x01));
  EXPECT_EQ(0x17f0, lcdc.bgTileDataAddress(0x7f));
  EXPECT_EQ(0x0800, lcdc.bgTileDataAddress(0x80));
  EXPECT_EQ(0x0810, lcdc.bgTileDataAddress(0x81));
  EXPECT_EQ(0x0ff0, lcdc.bgTileDataAddress(0xff));
}

TEST(LcdcTest, bgTileMapAddress) {
  Lcdc lcdc(0);

  lcdc.write(0b00000000);
  EXPECT_EQ(0x1800, lcdc.bgTileMapAddress(0x0000));
  EXPECT_EQ(0x1801, lcdc.bgTileMapAddress(0x0001));
  EXPECT_EQ(0x1bff, lcdc.bgTileMapAddress(0x03ff));

  lcdc.write(0b00001000);
  EXPECT_EQ(0x1c00, lcdc.bgTileMapAddress(0x0000));
  EXPECT_EQ(0x1c01, lcdc.bgTileMapAddress(0x0001));
  EXPECT_EQ(0x1fff, lcdc.bgTileMapAddress(0x03ff));
}

}  // namespace gbemu
