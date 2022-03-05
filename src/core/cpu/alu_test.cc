#include "core/cpu/alu.h"

#include <gtest/gtest.h>

namespace gbeml {

TEST(AluTest, add_n_setsZFlagIfResultIsZero) {
  RegisterPair af;
  Alu alu(&af);

  alu.add_n(0xff);
  EXPECT_EQ(false, alu.get_z());

  alu.add_n(1);
  EXPECT_EQ(true, alu.get_z());
}

TEST(AluTest, add_n_resetsN) {
  RegisterPair af;
  Alu alu(&af);

  alu.set_n(true);
  alu.add_n(1);
  EXPECT_EQ(false, alu.get_n());
}

TEST(AluTest, add_n_setsHIfCarryFromBit3) {
  RegisterPair af;
  Alu alu(&af);
  alu.set_a(0b1110);

  alu.add_n(1);
  EXPECT_EQ(false, alu.get_h());
  alu.add_n(1);
  EXPECT_EQ(true, alu.get_h());
}

TEST(AluTest, add_n_setsCIfCarryFromBit7) {
  RegisterPair af;
  Alu alu(&af);
  alu.set_a(0xfe);

  alu.add_n(1);
  EXPECT_EQ(false, alu.get_c());
  alu.add_n(1);
  EXPECT_EQ(true, alu.get_c());
}

TEST(AluTest, addc_n_addsNPlusC) {
  RegisterPair af;
  Alu alu(&af);

  alu.set_c(true);
  alu.addc_n(0xfe);
  EXPECT_EQ(0xff, alu.get_a());
}

TEST(AluTest, sub_n_setsZFlagIfResultIsZero) {
  RegisterPair af;
  Alu alu(&af);
  alu.set_a(0xff);

  alu.sub_n(0xfe);
  EXPECT_EQ(false, alu.get_z());

  alu.sub_n(1);
  EXPECT_EQ(true, alu.get_z());
}

TEST(AluTest, sub_n_setsN) {
  RegisterPair af;
  Alu alu(&af);

  EXPECT_EQ(false, alu.get_n());
  alu.sub_n(1);
  EXPECT_EQ(true, alu.get_n());
}

TEST(AluTest, sub_n_setsHIfNoBorrowBit4) {
  RegisterPair af;
  Alu alu(&af);
  alu.set_a(0xff);

  alu.sub_n(1);
  EXPECT_EQ(false, alu.get_h());
  alu.sub_n(0xf);
  EXPECT_EQ(true, alu.get_h());
}

TEST(AluTest, sub_n_setsCIfNoBorrow) {
  RegisterPair af;
  Alu alu(&af);
  alu.set_a(0xff);

  alu.sub_n(1);
  EXPECT_EQ(false, alu.get_c());
  alu.sub_n(0xff);
  EXPECT_EQ(true, alu.get_c());
}

TEST(AluTest, subc_n_subtractsNPlusC) {
  RegisterPair af;
  Alu alu(&af);
  alu.set_a(0xff);

  alu.set_c(true);
  alu.subc_n(0xfe);
  EXPECT_EQ(0, alu.get_a());
}

TEST(AluTest, and_n_setsZIfResultIsZero) {
  RegisterPair af;
  Alu alu(&af);
  alu.set_a(0xff);

  alu.and_n(0b10101010);
  EXPECT_EQ(false, alu.get_z());
  alu.and_n(0b01010101);
  EXPECT_EQ(true, alu.get_z());
}

TEST(AluTest, and_n_resetsN) {
  RegisterPair af;
  Alu alu(&af);

  alu.set_n(true);
  alu.and_n(0);
  EXPECT_EQ(false, alu.get_n());
}

TEST(AluTest, and_n_setsH) {
  RegisterPair af;
  Alu alu(&af);

  EXPECT_EQ(false, alu.get_h());
  alu.and_n(0);
  EXPECT_EQ(true, alu.get_h());
}

TEST(AluTest, and_n_resetsC) {
  RegisterPair af;
  Alu alu(&af);

  alu.set_c(true);
  alu.and_n(0);
  EXPECT_EQ(false, alu.get_c());
}

TEST(AluTest, or_n_setsZIfResultIsZero) {
  RegisterPair af;
  Alu alu(&af);

  alu.or_n(0);
  EXPECT_EQ(true, alu.get_z());
  alu.or_n(1);
  EXPECT_EQ(false, alu.get_z());
}

TEST(AluTest, or_n_resetsN) {
  RegisterPair af;
  Alu alu(&af);

  alu.set_n(true);
  alu.or_n(0);
  EXPECT_EQ(false, alu.get_n());
}

TEST(AluTest, or_n_resetsH) {
  RegisterPair af;
  Alu alu(&af);

  alu.set_h(true);
  alu.or_n(0);
  EXPECT_EQ(false, alu.get_h());
}

TEST(AluTest, or_n_resetsC) {
  RegisterPair af;
  Alu alu(&af);

  alu.set_c(true);
  alu.or_n(0);
  EXPECT_EQ(false, alu.get_c());
}

TEST(AluTest, xor_n_setsZIfResultIsZero) {
  RegisterPair af;
  Alu alu(&af);

  alu.xor_n(0);
  EXPECT_EQ(true, alu.get_z());
  alu.xor_n(0b1010);
  EXPECT_EQ(false, alu.get_z());
  alu.xor_n(0b0101);
  EXPECT_EQ(false, alu.get_z());
  alu.xor_n(0b1111);
  EXPECT_EQ(true, alu.get_z());
}

TEST(AluTest, xor_n_resetsN) {
  RegisterPair af;
  Alu alu(&af);

  alu.set_n(true);
  alu.xor_n(0);
  EXPECT_EQ(false, alu.get_n());
}

TEST(AluTest, xor_n_resetsH) {
  RegisterPair af;
  Alu alu(&af);

  alu.set_h(true);
  alu.xor_n(0);
  EXPECT_EQ(false, alu.get_h());
}

TEST(AluTest, xor_n_resetsC) {
  RegisterPair af;
  Alu alu(&af);

  alu.set_c(true);
  alu.xor_n(0);
  EXPECT_EQ(false, alu.get_c());
}

TEST(AluTest, add_hl_n16_resetsN) {
  RegisterPair af;
  Alu alu(&af);
  RegisterPair hl;

  alu.set_n(true);
  alu.add_hl_n16(&hl, 0);
  EXPECT_EQ(false, alu.get_n());
}

TEST(AluTest, add_hl_n16_setsHIfCarryFromBit11) {
  RegisterPair af;
  Alu alu(&af);
  RegisterPair hl(0x0f, 0);

  EXPECT_EQ(false, alu.get_h());
  alu.add_hl_n16(&hl, 0x0100);
  EXPECT_EQ(true, alu.get_h());
}

TEST(AluTest, add_hl_n16_setsCIfCarryFromBit15) {
  RegisterPair af;
  Alu alu(&af);
  RegisterPair hl(0xff, 1);

  EXPECT_EQ(false, alu.get_c());
  alu.add_hl_n16(&hl, 0xff);
  EXPECT_EQ(true, alu.get_c());
}

TEST(AluTest, add_sp_n8_resetsZ) {
  RegisterPair af;
  Alu alu(&af);
  RegisterPair hl;

  alu.set_z(true);
  alu.add_sp_n8(&hl, 0);
  EXPECT_EQ(false, alu.get_z());
}

TEST(AluTest, add_sp_n8_resetsN) {
  RegisterPair af;
  Alu alu(&af);
  RegisterPair hl;

  alu.set_n(true);
  alu.add_sp_n8(&hl, 0);
  EXPECT_EQ(false, alu.get_n());
}

TEST(AluTest, add_sp_n8_setsHIfCarryFromBit3) {
  RegisterPair af;
  Alu alu(&af);
  RegisterPair hl(0, 0x0f);

  EXPECT_EQ(false, alu.get_h());
  alu.add_sp_n8(&hl, 0x01);
  EXPECT_EQ(true, alu.get_h());
}

TEST(AluTest, add_sp_n8_setsCIfCarryFromBit7) {
  RegisterPair af;
  Alu alu(&af);
  RegisterPair hl(0, 0xff);

  EXPECT_EQ(false, alu.get_c());
  alu.add_sp_n8(&hl, 0xff);
  EXPECT_EQ(true, alu.get_c());
}

TEST(AluTest, daa_decimalAdjustAfterAddition) {
  RegisterPair af;
  Alu alu(&af);

  // 10
  alu.set_a(0x0a);
  alu.set_c(false);
  alu.set_h(false);
  alu.set_n(false);
  alu.daa();
  EXPECT_EQ(0x10, alu.get_a());
  EXPECT_EQ(false, alu.get_c());
  EXPECT_EQ(false, alu.get_h());
  EXPECT_EQ(false, alu.get_z());

  // 19
  alu.set_a(0x13);
  alu.set_c(false);
  alu.set_h(true);
  alu.set_n(false);
  alu.daa();
  EXPECT_EQ(0x19, alu.get_a());
  EXPECT_EQ(false, alu.get_c());
  EXPECT_EQ(false, alu.get_h());
  EXPECT_EQ(false, alu.get_z());

  // 128
  alu.set_a(0xc8);
  alu.set_c(false);
  alu.set_h(false);
  alu.set_n(false);
  alu.daa();
  EXPECT_EQ(0x28, alu.get_a());
  EXPECT_EQ(true, alu.get_c());
  EXPECT_EQ(false, alu.get_h());
  EXPECT_EQ(false, alu.get_z());

  // 180
  alu.set_a(0x20);
  alu.set_c(true);
  alu.set_h(false);
  alu.set_n(false);
  alu.daa();
  EXPECT_EQ(0x80, alu.get_a());
  EXPECT_EQ(true, alu.get_c());
  EXPECT_EQ(false, alu.get_h());
  EXPECT_EQ(false, alu.get_z());
}

TEST(AluTest, daa_decimalAdjustAfterSubtraction) {
  RegisterPair af;
  Alu alu(&af);

  // 9
  alu.set_a(0x0f);
  alu.set_c(false);
  alu.set_h(false);
  alu.set_n(true);
  alu.daa();
  EXPECT_EQ(0x09, alu.get_a());
  EXPECT_EQ(false, alu.get_c());
  EXPECT_EQ(false, alu.get_h());
  EXPECT_EQ(false, alu.get_z());

  // 1
  alu.set_a(0x07);
  alu.set_c(false);
  alu.set_h(true);
  alu.set_n(true);
  alu.daa();
  EXPECT_EQ(0x01, alu.get_a());
  EXPECT_EQ(false, alu.get_c());
  EXPECT_EQ(false, alu.get_h());
  EXPECT_EQ(false, alu.get_z());

  // 90
  alu.set_a(0xf0);
  alu.set_c(false);
  alu.set_h(false);
  alu.set_n(true);
  alu.daa();
  EXPECT_EQ(0x90, alu.get_a());
  EXPECT_EQ(false, alu.get_c());
  EXPECT_EQ(false, alu.get_h());
  EXPECT_EQ(false, alu.get_z());

  // 10
  alu.set_a(0x70);
  alu.set_c(true);
  alu.set_h(false);
  alu.set_n(true);
  alu.daa();
  EXPECT_EQ(0x10, alu.get_a());
  EXPECT_EQ(false, alu.get_c());
  EXPECT_EQ(false, alu.get_h());
  EXPECT_EQ(false, alu.get_z());
}

}  // namespace gbeml
