#include "core/graphics/pixel_fifo.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "core/graphics/color.h"

namespace gbeml {

TEST(PixelFifoTest, popPixel) {
  PixelFifo fifo;
  fifo.setPalette(0b11100100);
  fifo.pushTileData(0b01010101, 0b00110011);

  EXPECT_EQ(Color::White, fifo.popPixel());
  EXPECT_EQ(Color::LightGray, fifo.popPixel());
  EXPECT_EQ(Color::DarkGray, fifo.popPixel());
  EXPECT_EQ(Color::Black, fifo.popPixel());
}

}  // namespace gbeml
