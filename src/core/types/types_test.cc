#include "core/types/types.h"

#include <gtest/gtest.h>

namespace gbeml {

TEST(TypesTest, concat) {
  u16 n = concat(0xff, 0x11);
  EXPECT_EQ(0xff11, n);
}

}  // namespace gbeml
