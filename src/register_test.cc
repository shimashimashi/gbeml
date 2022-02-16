#include "register.h"

#include <gtest/gtest.h>

namespace gbemu {

TEST(RegisterTest, get) {
  Register* r = new Register();
  r->set(0xffff);
  EXPECT_EQ(0xffff, r->get());
}

TEST(RegisterTest, getLow) {
  Register* r = new Register();
  r->set(0xff11);
  EXPECT_EQ(0x11, r->getLow());
}

TEST(RegisterTest, getHigh) {
  Register* r = new Register();
  r->set(0xff11);
  EXPECT_EQ(0xff, r->getHigh());
}

TEST(RegisterTest, getAt) {
  Register* r = new Register();
  r->set(0b10010001);
  EXPECT_EQ(true, r->getAt(0));
  EXPECT_EQ(false, r->getAt(1));
  EXPECT_EQ(false, r->getAt(2));
  EXPECT_EQ(false, r->getAt(3));
  EXPECT_EQ(true, r->getAt(4));
  EXPECT_EQ(false, r->getAt(5));
  EXPECT_EQ(false, r->getAt(6));
  EXPECT_EQ(true, r->getAt(7));
}

TEST(RegisterTest, set) {
  Register* r = new Register();
  r->set(0xffff);
  r->set(0x1111);
  EXPECT_EQ(0x1111, r->get());
}

TEST(RegisterTest, setLow) {
  Register* r = new Register();
  r->set(0xffff);
  r->setLow(0x11);
  EXPECT_EQ(0xff11, r->get());
}

TEST(RegisterTest, setHigh) {
  Register* r = new Register();
  r->set(0xffff);
  r->setHigh(0x11);
  EXPECT_EQ(0x11ff, r->get());
}

TEST(RegisterTest, setAt) {
  Register* r = new Register();

  r->set(0b00000000);
  r->setAt(0, true);
  EXPECT_EQ(0b00000001, r->get());
  r->setAt(0, false);
  EXPECT_EQ(0b00000000, r->get());

  r->setAt(3, true);
  EXPECT_EQ(0b00001000, r->get());
  r->setAt(3, false);
  EXPECT_EQ(0b00000000, r->get());

  r->setAt(7, true);
  EXPECT_EQ(0b10000000, r->get());
  r->setAt(7, false);
  EXPECT_EQ(0b00000000, r->get());
}

TEST(RegisterTest, increment) {
  Register* r = new Register();

  r->set(0b00000000);
  r->increment();
  EXPECT_EQ(0b00000001, r->get());
}

TEST(RegisterTest, decrement) {
  Register* r = new Register();

  r->set(0b00000001);
  r->decrement();
  EXPECT_EQ(0b00000000, r->get());
}

}  // namespace gbemu
