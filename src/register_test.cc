#include "register.h"

#include <gtest/gtest.h>

namespace gbemu {

TEST(HalfRegisterTest, get) {
  HalfRegister* r = new HalfRegister();
  r->set(0xff);
  EXPECT_EQ(0xff, r->get());
}

TEST(HalfRegisterTest, getAt) {
  HalfRegister* r = new HalfRegister();
  r->set(0b10001011);
  EXPECT_EQ(true, r->getAt(0));
  EXPECT_EQ(true, r->getAt(1));
  EXPECT_EQ(false, r->getAt(2));
  EXPECT_EQ(true, r->getAt(3));
  EXPECT_EQ(false, r->getAt(4));
  EXPECT_EQ(false, r->getAt(5));
  EXPECT_EQ(false, r->getAt(6));
  EXPECT_EQ(true, r->getAt(7));
}

TEST(HalfRegisterTest, set) {
  HalfRegister* r = new HalfRegister();
  r->set(0xff);
  EXPECT_EQ(0xff, r->get());
  r->set(0x11);
  EXPECT_EQ(0x11, r->get());
}

TEST(HalfRegisterTest, setAt) {
  HalfRegister* r = new HalfRegister();

  r->set(0b0000);
  r->setAt(0, true);
  EXPECT_EQ(0b0001, r->get());
  r->setAt(0, false);
  EXPECT_EQ(0b0000, r->get());

  r->setAt(3, true);
  EXPECT_EQ(0b1000, r->get());
  r->setAt(3, false);
  EXPECT_EQ(0b0000, r->get());
}

TEST(HalfRegisterTest, increment) {
  HalfRegister* r = new HalfRegister();
  r->increment();
  EXPECT_EQ(1, r->get());
}

TEST(HalfRegisterTest, decrement) {
  HalfRegister* r = new HalfRegister();
  r->set(1);
  EXPECT_EQ(1, r->get());
  r->decrement();
  EXPECT_EQ(0, r->get());
}

TEST(RegisterTest, get) {
  HalfRegister* h = new HalfRegister();
  HalfRegister* l = new HalfRegister();
  h->set(0xff);
  l->set(0x11);
  Register* r = new Register(h, l);
  EXPECT_EQ(0xff11, r->get());
}

TEST(RegisterTest, set) {
  HalfRegister* h = new HalfRegister();
  HalfRegister* l = new HalfRegister();
  h->set(0xff);
  l->set(0x11);
  Register* r = new Register(h, l);
  r->set(0x11ff);
  EXPECT_EQ(0x11ff, r->get());
}

TEST(RegisterTest, increment) {
  HalfRegister* h = new HalfRegister();
  HalfRegister* l = new HalfRegister();
  Register* r = new Register(h, l);
  r->increment();
  EXPECT_EQ(1, r->get());
}

TEST(RegisterTest, decrement) {
  HalfRegister* h = new HalfRegister();
  HalfRegister* l = new HalfRegister();
  Register* r = new Register(h, l);
  r->set(1);
  EXPECT_EQ(1, r->get());
  r->decrement();
  EXPECT_EQ(0, r->get());
}

}  // namespace gbemu
