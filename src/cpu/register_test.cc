#include "cpu/register.h"

#include <gtest/gtest.h>

namespace gbemu {

TEST(RegisterTest, get) {
  Register* r = new Register(0xff);
  EXPECT_EQ(0xff, r->get());
}

TEST(RegisterTest, getAt) {
  Register* r = new Register(0b10001011);
  EXPECT_EQ(true, r->getAt(0));
  EXPECT_EQ(true, r->getAt(1));
  EXPECT_EQ(false, r->getAt(2));
  EXPECT_EQ(true, r->getAt(3));
  EXPECT_EQ(false, r->getAt(4));
  EXPECT_EQ(false, r->getAt(5));
  EXPECT_EQ(false, r->getAt(6));
  EXPECT_EQ(true, r->getAt(7));
}

TEST(RegisterTest, set) {
  Register* r = new Register(0xff);
  r->set(0x11);
  EXPECT_EQ(0x11, r->get());
}

TEST(RegisterTest, setAt) {
  Register* r = new Register();

  r->setAt(0, true);
  EXPECT_EQ(0b0001, r->get());
  r->setAt(0, false);
  EXPECT_EQ(0b0000, r->get());

  r->setAt(3, true);
  EXPECT_EQ(0b1000, r->get());
  r->setAt(3, false);
  EXPECT_EQ(0b0000, r->get());
}

TEST(RegisterTest, increment) {
  Register* r = new Register();
  r->increment();
  EXPECT_EQ(1, r->get());
}

TEST(RegisterTest, decrement) {
  Register* r = new Register(1);
  EXPECT_EQ(1, r->get());
  r->decrement();
  EXPECT_EQ(0, r->get());
}

TEST(RegisterPairTest, get) {
  Register* h = new Register(0xff);
  Register* l = new Register(0x11);
  RegisterPair* rp = new RegisterPair(h, l);
  EXPECT_EQ(0xff11, rp->get());
}

TEST(RegisterPairTest, set) {
  Register* h = new Register(0xff);
  Register* l = new Register(0x11);
  RegisterPair* rp = new RegisterPair(h, l);
  rp->set(0x11ff);
  EXPECT_EQ(0x11ff, rp->get());
}

TEST(RegisterPairTest, increment) {
  Register* h = new Register();
  Register* l = new Register();
  RegisterPair* rp = new RegisterPair(h, l);
  rp->increment();
  EXPECT_EQ(1, rp->get());
}

TEST(RegisterPairTest, decrement) {
  Register* h = new Register();
  Register* l = new Register(1);
  RegisterPair* rp = new RegisterPair(h, l);
  rp->decrement();
  EXPECT_EQ(0, rp->get());
}

TEST(RegisterPairTest, getHigh) {
  Register* h = new Register(1);
  Register* l = new Register();
  RegisterPair* rp = new RegisterPair(h, l);
  EXPECT_EQ(1, rp->getHigh()->get());
}

TEST(RegisterPairTest, getLow) {
  Register* h = new Register(0);
  Register* l = new Register(1);
  RegisterPair* rp = new RegisterPair(h, l);
  EXPECT_EQ(1, rp->getLow()->get());
}

}  // namespace gbemu
