#include "core/cpu/opcode.h"

#include <gtest/gtest.h>

namespace gbemu {

TEST(OpcodeTest, get) {
  Opcode opcode(0xff);
  EXPECT_EQ(0xff, opcode.get());
}

TEST(OpcodeTest, match) {
  Opcode opcode(0b11001101);
  EXPECT_EQ(true, opcode.match("11001101"));
  EXPECT_EQ(true, opcode.match("1100110x"));
  EXPECT_EQ(true, opcode.match("110xxx0x"));
  EXPECT_EQ(false, opcode.match("110xxx1x"));
  EXPECT_EQ(false, opcode.match("111xxx0x"));
}

TEST(OpcodeTest, slice) {
  Opcode opcode(0b11001101);
  EXPECT_EQ(0b1101, opcode.slice(0, 3));
  EXPECT_EQ(0b0011, opcode.slice(2, 3));
  EXPECT_EQ(0b1100, opcode.slice(4, 7));
}

}  // namespace gbemu
