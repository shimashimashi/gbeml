#include "core/graphics/sprite.h"

#include <gtest/gtest.h>

namespace gbeml {

TEST(SpriteTest, isVisibleVertically) {
  Sprite sprite_x_zero(16, 0, 0, 0);
  EXPECT_EQ(false, sprite_x_zero.isVisibleVertically(0, SpriteSize::Tall));

  Sprite sprite(17, 8, 0, 0);
  EXPECT_EQ(false, sprite.isVisibleVertically(0, SpriteSize::Tall));
  EXPECT_EQ(true, sprite.isVisibleVertically(1, SpriteSize::Tall));
  EXPECT_EQ(true, sprite.isVisibleVertically(16, SpriteSize::Tall));
  EXPECT_EQ(false, sprite.isVisibleVertically(17, SpriteSize::Tall));

  EXPECT_EQ(false, sprite.isVisibleVertically(0, SpriteSize::Short));
  EXPECT_EQ(true, sprite.isVisibleVertically(1, SpriteSize::Short));
  EXPECT_EQ(true, sprite.isVisibleVertically(8, SpriteSize::Short));
  EXPECT_EQ(false, sprite.isVisibleVertically(9, SpriteSize::Short));
}

TEST(SpriteTest, getTileDataAddress) {
  Sprite sprite_not_flip(17, 8, 0xff, 0);
  EXPECT_EQ(0xfe0, sprite_not_flip.getTileDataAddress(1, SpriteSize::Tall));
  EXPECT_EQ(0xffe, sprite_not_flip.getTileDataAddress(16, SpriteSize::Tall));
  EXPECT_EQ(0xff0, sprite_not_flip.getTileDataAddress(1, SpriteSize::Short));
  EXPECT_EQ(0xffe, sprite_not_flip.getTileDataAddress(8, SpriteSize::Short));

  Sprite sprite_flip(17, 8, 0xff, 0b01000000);
  EXPECT_EQ(0xfe0, sprite_flip.getTileDataAddress(16, SpriteSize::Tall));
  EXPECT_EQ(0xffe, sprite_flip.getTileDataAddress(1, SpriteSize::Tall));
  EXPECT_EQ(0xffe, sprite_flip.getTileDataAddress(1, SpriteSize::Short));
  EXPECT_EQ(0xff0, sprite_flip.getTileDataAddress(8, SpriteSize::Short));

  EXPECT_EQ(0xff, sprite_flip.getTileIndex());
  EXPECT_EQ(17, sprite_flip.getY());
}

TEST(SpriteTest, getPixelIndex) {
  Sprite sprite_not_flip(16, 8, 0xff, 0);
  EXPECT_EQ(0, sprite_not_flip.getPixelIndex(0));
  EXPECT_EQ(7, sprite_not_flip.getPixelIndex(7));

  Sprite sprite_flip(16, 8, 0xff, 0b00100000);
  EXPECT_EQ(7, sprite_flip.getPixelIndex(0));
  EXPECT_EQ(0, sprite_flip.getPixelIndex(7));
}

}  // namespace gbeml
