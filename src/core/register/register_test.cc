#include "core/register/register.h"

#include <gtest/gtest.h>

namespace gbeml {

TEST(RegisterTest, get) {
  Register r(0xff);
  EXPECT_EQ(0xff, r.get());
}

TEST(RegisterTest, getAt) {
  Register r(0b10001011);
  EXPECT_EQ(true, r.getAt(0));
  EXPECT_EQ(true, r.getAt(1));
  EXPECT_EQ(false, r.getAt(2));
  EXPECT_EQ(true, r.getAt(3));
  EXPECT_EQ(false, r.getAt(4));
  EXPECT_EQ(false, r.getAt(5));
  EXPECT_EQ(false, r.getAt(6));
  EXPECT_EQ(true, r.getAt(7));
}

TEST(RegisterTest, set) {
  Register r(0xff);
  r.set(0x11);
  EXPECT_EQ(0x11, r.get());
}

TEST(RegisterTest, setAt) {
  Register r;

  r.setAt(0, true);
  EXPECT_EQ(0b0001, r.get());
  r.setAt(0, false);
  EXPECT_EQ(0b0000, r.get());

  r.setAt(3, true);
  EXPECT_EQ(0b1000, r.get());
  r.setAt(3, false);
  EXPECT_EQ(0b0000, r.get());
}

TEST(RegisterTest, increment) {
  Register r;
  r.increment();
  EXPECT_EQ(1, r.get());
}

TEST(RegisterTest, decrement) {
  Register r(1);
  EXPECT_EQ(1, r.get());
  r.decrement();
  EXPECT_EQ(0, r.get());
}

TEST(RegisterPairTest, get) {
  RegisterPair rp(0xff, 0x11);
  EXPECT_EQ(0xff11, rp.get());
}

TEST(RegisterPairTest, set) {
  RegisterPair rp(0xff, 0x11);
  rp.set(0x11ff);
  EXPECT_EQ(0x11ff, rp.get());
}

TEST(RegisterPairTest, increment) {
  RegisterPair rp;
  rp.increment();
  EXPECT_EQ(1, rp.get());
}

TEST(RegisterPairTest, decrement) {
  RegisterPair rp(0, 1);
  rp.decrement();
  EXPECT_EQ(0, rp.get());
}

TEST(RegisterPairTest, getHigh) {
  RegisterPair rp(1, 0);
  EXPECT_EQ(1, rp.getHigh()->get());
}

TEST(RegisterPairTest, getLow) {
  RegisterPair rp(0, 1);
  EXPECT_EQ(1, rp.getLow()->get());
}

}  // namespace gbeml
