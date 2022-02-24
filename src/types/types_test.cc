#include "types/types.h"

#include <gtest/gtest.h>

namespace gbemu {

TEST(TypesTest, concat) {
  u16 n = concat(0xff, 0x11);
  EXPECT_EQ(0xff11, n);
}

}  // namespace gbemu
