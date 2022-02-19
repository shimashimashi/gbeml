#include "cpu/alu.h"

#include <gtest/gtest.h>

namespace gbemu {

TEST(AluTest, add_n_setsZFlagIfResultIsZero) {
  Register* a = new Register();
  Register* f = new Register();
  RegisterPair* af = new RegisterPair(a, f);
  Alu* alu = new Alu(af);

  alu->add_n(0xff);
  EXPECT_EQ(false, alu->get_z());

  alu->add_n(1);
  EXPECT_EQ(true, alu->get_z());
}

TEST(AluTest, add_n_resetsN) {
  Register* a = new Register();
  Register* f = new Register();
  RegisterPair* af = new RegisterPair(a, f);
  Alu* alu = new Alu(af);

  alu->set_n(true);
  alu->add_n(1);
  EXPECT_EQ(false, alu->get_n());
}

TEST(AluTest, add_n_setsHIfCarryFromBit3) {
  Register* a = new Register(0b1110);
  Register* f = new Register();
  RegisterPair* af = new RegisterPair(a, f);
  Alu* alu = new Alu(af);

  alu->add_n(1);
  EXPECT_EQ(false, alu->get_h());
  alu->add_n(1);
  EXPECT_EQ(true, alu->get_h());
}

TEST(AluTest, add_n_setsCIfCarryFromBit7) {
  Register* a = new Register(0xfe);
  Register* f = new Register();
  RegisterPair* af = new RegisterPair(a, f);
  Alu* alu = new Alu(af);

  alu->add_n(1);
  EXPECT_EQ(false, alu->get_c());
  alu->add_n(1);
  EXPECT_EQ(true, alu->get_c());
}

TEST(AluTest, addc_n_addsNPlusC) {
  Register* a = new Register(0);
  Register* f = new Register();
  RegisterPair* af = new RegisterPair(a, f);
  Alu* alu = new Alu(af);

  alu->set_c(true);
  alu->addc_n(0xfe);
  EXPECT_EQ(0xff, alu->get_a());
}

TEST(AluTest, sub_n_setsZFlagIfResultIsZero) {
  Register* a = new Register(0xff);
  Register* f = new Register();
  RegisterPair* af = new RegisterPair(a, f);
  Alu* alu = new Alu(af);

  alu->sub_n(0xfe);
  EXPECT_EQ(false, alu->get_z());

  alu->sub_n(1);
  EXPECT_EQ(true, alu->get_z());
}

TEST(AluTest, sub_n_setsN) {
  Register* a = new Register();
  Register* f = new Register();
  RegisterPair* af = new RegisterPair(a, f);
  Alu* alu = new Alu(af);

  EXPECT_EQ(false, alu->get_n());
  alu->sub_n(1);
  EXPECT_EQ(true, alu->get_n());
}

TEST(AluTest, sub_n_setsHIfNoBorrowBit4) {
  Register* a = new Register(0x10);
  Register* f = new Register();
  RegisterPair* af = new RegisterPair(a, f);
  Alu* alu = new Alu(af);

  alu->sub_n(0xf);
  EXPECT_EQ(true, alu->get_h());
  alu->sub_n(1);
  EXPECT_EQ(false, alu->get_h());
}

TEST(AluTest, sub_n_setsCIfNoBorrow) {
  Register* a = new Register(0xff);
  Register* f = new Register();
  RegisterPair* af = new RegisterPair(a, f);
  Alu* alu = new Alu(af);

  alu->sub_n(1);
  EXPECT_EQ(false, alu->get_c());
  alu->sub_n(0xff);
  EXPECT_EQ(true, alu->get_c());
}

TEST(AluTest, subc_n_subtractsNPlusC) {
  Register* a = new Register(0xff);
  Register* f = new Register();
  RegisterPair* af = new RegisterPair(a, f);
  Alu* alu = new Alu(af);

  alu->set_c(true);
  alu->subc_n(0xfe);
  EXPECT_EQ(0, alu->get_a());
}

TEST(AluTest, and_n_setsZIfResultIsZero) {
  Register* a = new Register(0xff);
  Register* f = new Register();
  RegisterPair* af = new RegisterPair(a, f);
  Alu* alu = new Alu(af);

  alu->and_n(0b10101010);
  EXPECT_EQ(false, alu->get_z());
  alu->and_n(0b01010101);
  EXPECT_EQ(true, alu->get_z());
}

TEST(AluTest, and_n_resetsN) {
  Register* a = new Register();
  Register* f = new Register();
  RegisterPair* af = new RegisterPair(a, f);
  Alu* alu = new Alu(af);

  alu->set_n(true);
  alu->and_n(0);
  EXPECT_EQ(false, alu->get_n());
}

TEST(AluTest, and_n_setsH) {
  Register* a = new Register();
  Register* f = new Register();
  RegisterPair* af = new RegisterPair(a, f);
  Alu* alu = new Alu(af);

  EXPECT_EQ(false, alu->get_h());
  alu->and_n(0);
  EXPECT_EQ(true, alu->get_h());
}

TEST(AluTest, and_n_resetsC) {
  Register* a = new Register();
  Register* f = new Register();
  RegisterPair* af = new RegisterPair(a, f);
  Alu* alu = new Alu(af);

  alu->set_c(true);
  alu->and_n(0);
  EXPECT_EQ(false, alu->get_c());
}

TEST(AluTest, or_n_setsZIfResultIsZero) {
  Register* a = new Register();
  Register* f = new Register();
  RegisterPair* af = new RegisterPair(a, f);
  Alu* alu = new Alu(af);

  alu->or_n(0);
  EXPECT_EQ(true, alu->get_z());
  alu->or_n(1);
  EXPECT_EQ(false, alu->get_z());
}

TEST(AluTest, or_n_resetsN) {
  Register* a = new Register();
  Register* f = new Register();
  RegisterPair* af = new RegisterPair(a, f);
  Alu* alu = new Alu(af);

  alu->set_n(true);
  alu->or_n(0);
  EXPECT_EQ(false, alu->get_n());
}

TEST(AluTest, or_n_resetsH) {
  Register* a = new Register();
  Register* f = new Register();
  RegisterPair* af = new RegisterPair(a, f);
  Alu* alu = new Alu(af);

  alu->set_h(true);
  alu->or_n(0);
  EXPECT_EQ(false, alu->get_h());
}

TEST(AluTest, or_n_resetsC) {
  Register* a = new Register();
  Register* f = new Register();
  RegisterPair* af = new RegisterPair(a, f);
  Alu* alu = new Alu(af);

  alu->set_c(true);
  alu->or_n(0);
  EXPECT_EQ(false, alu->get_c());
}

TEST(AluTest, xor_n_setsZIfResultIsZero) {
  Register* a = new Register();
  Register* f = new Register();
  RegisterPair* af = new RegisterPair(a, f);
  Alu* alu = new Alu(af);

  alu->xor_n(0);
  EXPECT_EQ(true, alu->get_z());
  alu->xor_n(0b1010);
  EXPECT_EQ(false, alu->get_z());
  alu->xor_n(0b0101);
  EXPECT_EQ(false, alu->get_z());
  alu->xor_n(0b1111);
  EXPECT_EQ(true, alu->get_z());
}

TEST(AluTest, xor_n_resetsN) {
  Register* a = new Register();
  Register* f = new Register();
  RegisterPair* af = new RegisterPair(a, f);
  Alu* alu = new Alu(af);

  alu->set_n(true);
  alu->xor_n(0);
  EXPECT_EQ(false, alu->get_n());
}

TEST(AluTest, xor_n_resetsH) {
  Register* a = new Register();
  Register* f = new Register();
  RegisterPair* af = new RegisterPair(a, f);
  Alu* alu = new Alu(af);

  alu->set_h(true);
  alu->xor_n(0);
  EXPECT_EQ(false, alu->get_h());
}

TEST(AluTest, xor_n_resetsC) {
  Register* a = new Register();
  Register* f = new Register();
  RegisterPair* af = new RegisterPair(a, f);
  Alu* alu = new Alu(af);

  alu->set_c(true);
  alu->xor_n(0);
  EXPECT_EQ(false, alu->get_c());
}

TEST(AluTest, add_hl_n16_resetsN) {
  Register* a = new Register();
  Register* f = new Register();
  RegisterPair* af = new RegisterPair(a, f);
  Alu* alu = new Alu(af);

  Register* h = new Register();
  Register* l = new Register();
  RegisterPair* hl = new RegisterPair(h, l);

  alu->set_n(true);
  alu->add_hl_n16(hl, 0);
  EXPECT_EQ(false, alu->get_n());
}

TEST(AluTest, add_hl_n16_setsHIfCarryFromBit11) {
  Register* a = new Register();
  Register* f = new Register();
  RegisterPair* af = new RegisterPair(a, f);
  Alu* alu = new Alu(af);

  Register* h = new Register(0x0f);
  Register* l = new Register();
  RegisterPair* hl = new RegisterPair(h, l);

  EXPECT_EQ(false, alu->get_h());
  alu->add_hl_n16(hl, 0x0100);
  EXPECT_EQ(true, alu->get_h());
}

TEST(AluTest, add_hl_n16_setsCIfCarryFromBit15) {
  Register* a = new Register();
  Register* f = new Register();
  RegisterPair* af = new RegisterPair(a, f);
  Alu* alu = new Alu(af);

  Register* h = new Register(0xff);
  Register* l = new Register(1);
  RegisterPair* hl = new RegisterPair(h, l);

  EXPECT_EQ(false, alu->get_c());
  alu->add_hl_n16(hl, 0xff);
  EXPECT_EQ(true, alu->get_c());
}

TEST(AluTest, add_sp_n8_resetsZ) {
  Register* a = new Register();
  Register* f = new Register();
  RegisterPair* af = new RegisterPair(a, f);
  Alu* alu = new Alu(af);

  Register* h = new Register();
  Register* l = new Register();
  RegisterPair* hl = new RegisterPair(h, l);

  alu->set_z(true);
  alu->add_sp_n8(hl, 0);
  EXPECT_EQ(false, alu->get_z());
}

TEST(AluTest, add_sp_n8_resetsN) {
  Register* a = new Register();
  Register* f = new Register();
  RegisterPair* af = new RegisterPair(a, f);
  Alu* alu = new Alu(af);

  Register* h = new Register();
  Register* l = new Register();
  RegisterPair* hl = new RegisterPair(h, l);

  alu->set_n(true);
  alu->add_sp_n8(hl, 0);
  EXPECT_EQ(false, alu->get_n());
}

TEST(AluTest, add_sp_n8_setsHIfCarryFromBit3) {
  Register* a = new Register();
  Register* f = new Register();
  RegisterPair* af = new RegisterPair(a, f);
  Alu* alu = new Alu(af);

  Register* h = new Register();
  Register* l = new Register(0x0f);
  RegisterPair* hl = new RegisterPair(h, l);

  EXPECT_EQ(false, alu->get_h());
  alu->add_sp_n8(hl, 0x01);
  EXPECT_EQ(true, alu->get_h());
}

TEST(AluTest, add_sp_n8_setsCIfCarryFromBit7) {
  Register* a = new Register();
  Register* f = new Register();
  RegisterPair* af = new RegisterPair(a, f);
  Alu* alu = new Alu(af);

  Register* h = new Register();
  Register* l = new Register(0xff);
  RegisterPair* hl = new RegisterPair(h, l);

  EXPECT_EQ(false, alu->get_c());
  alu->add_sp_n8(hl, 0xff);
  EXPECT_EQ(true, alu->get_c());
}

TEST(AluTest, daa_decimalAdjustAfterAddition) {
  Register* a = new Register();
  Register* f = new Register();
  RegisterPair* af = new RegisterPair(a, f);
  Alu* alu = new Alu(af);

  // 10
  alu->set_a(0x0a);
  alu->set_c(false);
  alu->set_h(false);
  alu->set_n(false);
  alu->daa();
  EXPECT_EQ(0x10, alu->get_a());
  EXPECT_EQ(false, alu->get_c());
  EXPECT_EQ(false, alu->get_h());
  EXPECT_EQ(false, alu->get_z());

  // 19
  alu->set_a(0x13);
  alu->set_c(false);
  alu->set_h(true);
  alu->set_n(false);
  alu->daa();
  EXPECT_EQ(0x19, alu->get_a());
  EXPECT_EQ(false, alu->get_c());
  EXPECT_EQ(false, alu->get_h());
  EXPECT_EQ(false, alu->get_z());

  // 128
  alu->set_a(0xc8);
  alu->set_c(false);
  alu->set_h(false);
  alu->set_n(false);
  alu->daa();
  EXPECT_EQ(0x28, alu->get_a());
  EXPECT_EQ(true, alu->get_c());
  EXPECT_EQ(false, alu->get_h());
  EXPECT_EQ(false, alu->get_z());

  // 180
  alu->set_a(0x20);
  alu->set_c(true);
  alu->set_h(false);
  alu->set_n(false);
  alu->daa();
  EXPECT_EQ(0x80, alu->get_a());
  EXPECT_EQ(true, alu->get_c());
  EXPECT_EQ(false, alu->get_h());
  EXPECT_EQ(false, alu->get_z());
}

TEST(AluTest, daa_decimalAdjustAfterSubtraction) {
  Register* a = new Register();
  Register* f = new Register();
  RegisterPair* af = new RegisterPair(a, f);
  Alu* alu = new Alu(af);

  // 9
  alu->set_a(0x0f);
  alu->set_c(false);
  alu->set_h(false);
  alu->set_n(true);
  alu->daa();
  EXPECT_EQ(0x09, alu->get_a());
  EXPECT_EQ(false, alu->get_c());
  EXPECT_EQ(false, alu->get_h());
  EXPECT_EQ(false, alu->get_z());

  // 1
  alu->set_a(0x07);
  alu->set_c(false);
  alu->set_h(true);
  alu->set_n(true);
  alu->daa();
  EXPECT_EQ(0x01, alu->get_a());
  EXPECT_EQ(false, alu->get_c());
  EXPECT_EQ(false, alu->get_h());
  EXPECT_EQ(false, alu->get_z());

  // 90
  alu->set_a(0xf0);
  alu->set_c(false);
  alu->set_h(false);
  alu->set_n(true);
  alu->daa();
  EXPECT_EQ(0x90, alu->get_a());
  EXPECT_EQ(false, alu->get_c());
  EXPECT_EQ(false, alu->get_h());
  EXPECT_EQ(false, alu->get_z());

  // 10
  alu->set_a(0x70);
  alu->set_c(true);
  alu->set_h(false);
  alu->set_n(true);
  alu->daa();
  EXPECT_EQ(0x10, alu->get_a());
  EXPECT_EQ(false, alu->get_c());
  EXPECT_EQ(false, alu->get_h());
  EXPECT_EQ(false, alu->get_z());
}

}  // namespace gbemu
