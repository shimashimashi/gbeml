#include "core/display/display_impl.h"

#include <gtest/gtest.h>

#include "core/graphics/color.h"

namespace gbeml {

TEST(DisplayImplTest, render) {
  DisplayImpl display;

  display.render(0, 0, Color::White);
  display.render(1, 0, Color::LightGray);
  display.render(2, 0, Color::DarkGray);
  display.render(3, 0, Color::Black);

  u32* buffer = display.getBuffer();

  EXPECT_EQ(0xffffff, buffer[0]);
  EXPECT_EQ(0xd3d3d3, buffer[1]);
  EXPECT_EQ(0xa9a9a9, buffer[2]);
  EXPECT_EQ(0x000000, buffer[3]);
}

}  // namespace gbeml
