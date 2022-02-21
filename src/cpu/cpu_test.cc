#include "cpu/cpu.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "bus.h"
#include "types.h"

namespace gbemu {

class MockBus : public Bus {
 public:
  MOCK_CONST_METHOD1(read, u8(u16 addr));
  MOCK_METHOD2(write, void(u16 addr, u8 value));
};

void expectCycles(u8 n, Cpu* cpu) {
  for (i64 i = 0; i < n; ++i) {
    EXPECT_TRUE(cpu->stalled());
    cpu->tick();
  }
  EXPECT_FALSE(cpu->stalled());
}

TEST(CpuTest, nop) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x00));

  Cpu* cpu = new Cpu(&bus);
  cpu->tick();
  expectCycles(4, cpu);
}

TEST(CpuTest, load_bc_n16) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x01));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0x34));
  EXPECT_CALL(bus, read(2)).WillOnce(testing::Return(0x12));

  Cpu* cpu = new Cpu(&bus);
  cpu->tick();
  EXPECT_EQ(0x1234, cpu->get_bc());
  expectCycles(12, cpu);
}

TEST(CpuTest, load_de_n16) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x11));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0x34));
  EXPECT_CALL(bus, read(2)).WillOnce(testing::Return(0x12));

  Cpu* cpu = new Cpu(&bus);
  cpu->tick();
  EXPECT_EQ(0x1234, cpu->get_de());
  expectCycles(12, cpu);
}

TEST(CpuTest, load_hl_n16) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x21));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0x34));
  EXPECT_CALL(bus, read(2)).WillOnce(testing::Return(0x12));

  Cpu* cpu = new Cpu(&bus);
  cpu->tick();
  EXPECT_EQ(0x1234, cpu->get_hl());
  expectCycles(12, cpu);
}

TEST(CpuTest, load_sp_n16) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x31));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0x34));
  EXPECT_CALL(bus, read(2)).WillOnce(testing::Return(0x12));

  Cpu* cpu = new Cpu(&bus);
  cpu->tick();
  EXPECT_EQ(0x1234, cpu->get_sp());
  expectCycles(12, cpu);
}

TEST(CpuTest, load_bc_a) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x02));
  EXPECT_CALL(bus, write(0xffff, 0x11)).Times(1);

  Cpu* cpu = new Cpu(&bus);
  cpu->set_a(0x11);
  cpu->set_bc(0xffff);
  cpu->tick();
  expectCycles(8, cpu);
}

TEST(CpuTest, load_de_a) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x12));
  EXPECT_CALL(bus, write(0xffff, 0x11)).Times(1);

  Cpu* cpu = new Cpu(&bus);
  cpu->set_a(0x11);
  cpu->set_de(0xffff);
  cpu->tick();
  expectCycles(8, cpu);
}

TEST(CpuTest, load_hl_a_inc) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x22));
  EXPECT_CALL(bus, write(0xffff, 0x11)).Times(1);

  Cpu* cpu = new Cpu(&bus);
  cpu->set_a(0x11);
  cpu->set_hl(0xffff);
  cpu->tick();
  EXPECT_EQ(0, cpu->get_hl());
  expectCycles(8, cpu);
}

TEST(CpuTest, load_hl_a_dec) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x32));
  EXPECT_CALL(bus, write(0xffff, 0x11)).Times(1);

  Cpu* cpu = new Cpu(&bus);
  cpu->set_a(0x11);
  cpu->set_hl(0xffff);
  cpu->tick();
  EXPECT_EQ(0xfffe, cpu->get_hl());
  expectCycles(8, cpu);
}

TEST(CpuTest, inc_r16) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x03));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_bc(0xfffe);
  cpu->tick();
  EXPECT_EQ(0xffff, cpu->get_bc());
  expectCycles(4, cpu);
}

TEST(CpuTest, inc_r8) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x04));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_b(0xfe);
  cpu->tick();
  EXPECT_EQ(0xff, cpu->get_b());
  expectCycles(4, cpu);
}

TEST(CpuTest, dec_r8) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x15));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_d(0xff);
  cpu->tick();
  EXPECT_EQ(0xfe, cpu->get_d());
  expectCycles(4, cpu);
}

TEST(CpuTest, load_r_n8) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x0e));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0xff));

  Cpu* cpu = new Cpu(&bus);
  cpu->tick();
  EXPECT_EQ(0xff, cpu->get_c());
  expectCycles(8, cpu);
}

TEST(CpuTest, rlca) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x07));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_a(0b0100111);
  cpu->tick();
  EXPECT_EQ(0b1001110, cpu->get_a());
  expectCycles(4, cpu);
}

TEST(CpuTest, load_n16_sp) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x08));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0x34));
  EXPECT_CALL(bus, read(2)).WillOnce(testing::Return(0x12));
  EXPECT_CALL(bus, write(0xfffe, 0x34)).Times(1);
  EXPECT_CALL(bus, write(0xffff, 0x12)).Times(1);

  Cpu* cpu = new Cpu(&bus);
  cpu->set_sp(0xfffe);
  cpu->tick();
  expectCycles(20, cpu);
}

TEST(CpuTest, add_hl_r) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x09));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_bc(0x0fff);
  cpu->set_hl(0x0001);
  cpu->tick();
  EXPECT_EQ(0x1000, cpu->get_hl());
  expectCycles(8, cpu);
}

TEST(CpuTest, load_a_bc) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x0a));
  EXPECT_CALL(bus, read(0xffff)).WillOnce(testing::Return(0x34));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_bc(0xffff);
  cpu->tick();
  EXPECT_EQ(0x34, cpu->get_a());
  expectCycles(8, cpu);
}

TEST(CpuTest, load_a_de) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x1a));
  EXPECT_CALL(bus, read(0xffff)).WillOnce(testing::Return(0x34));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_de(0xffff);
  cpu->tick();
  EXPECT_EQ(0x34, cpu->get_a());
  expectCycles(8, cpu);
}

TEST(CpuTest, load_a_hl_inc) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x2a));
  EXPECT_CALL(bus, read(0xfffe)).WillOnce(testing::Return(0x34));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_hl(0xfffe);
  cpu->tick();
  EXPECT_EQ(0x34, cpu->get_a());
  EXPECT_EQ(0xffff, cpu->get_hl());
  expectCycles(8, cpu);
}

TEST(CpuTest, load_a_hl_dec) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x3a));
  EXPECT_CALL(bus, read(0xffff)).WillOnce(testing::Return(0x34));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_hl(0xffff);
  cpu->tick();
  EXPECT_EQ(0x34, cpu->get_a());
  EXPECT_EQ(0xfffe, cpu->get_hl());
  expectCycles(8, cpu);
}

TEST(CpuTest, dec_r16) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x0b));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_bc(0xffff);
  cpu->tick();
  EXPECT_EQ(0xfffe, cpu->get_bc());
  expectCycles(4, cpu);
}

TEST(CpuTest, rrca) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x0f));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_a(0b10110011);
  cpu->tick();
  EXPECT_EQ(0b11011001, cpu->get_a());
  expectCycles(4, cpu);
}

TEST(CpuTest, rla) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x17));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_a(0b0100111);
  cpu->set_carry(true);
  cpu->tick();
  EXPECT_EQ(0b1001111, cpu->get_a());
  expectCycles(4, cpu);
}

TEST(CpuTest, jr_n_pos) {
  MockBus bus;
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0x18));
  EXPECT_CALL(bus, read(2)).WillOnce(testing::Return(0x04));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_pc(1);
  cpu->tick();
  EXPECT_EQ(7, cpu->get_pc());
  expectCycles(8, cpu);
}

TEST(CpuTest, jr_n_neg) {
  MockBus bus;
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0x18));
  EXPECT_CALL(bus, read(2)).WillOnce(testing::Return(0xfe));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_pc(1);
  cpu->tick();
  EXPECT_EQ(1, cpu->get_pc());
  expectCycles(8, cpu);
}

TEST(CpuTest, rra) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x1f));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_a(0b00010111);
  cpu->set_carry(true);
  cpu->tick();
  EXPECT_EQ(0b10001011, cpu->get_a());
  expectCycles(4, cpu);
}

TEST(CpuTest, jr_cc_n) {
  MockBus bus;
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0x20));
  EXPECT_CALL(bus, read(2)).WillOnce(testing::Return(0xfe));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_pc(1);
  cpu->tick();
  EXPECT_EQ(1, cpu->get_pc());
  expectCycles(8, cpu);
}

TEST(CpuTest, daa) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x27));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_a(0x0a);
  cpu->tick();
  EXPECT_EQ(0x10, cpu->get_a());
  expectCycles(4, cpu);
}

TEST(CpuTest, cpl) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x2f));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_a(0b10101010);
  cpu->tick();
  EXPECT_EQ(0b01010101, cpu->get_a());
  expectCycles(4, cpu);
}

TEST(CpuTest, scf) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x37));

  Cpu* cpu = new Cpu(&bus);
  cpu->tick();
  EXPECT_TRUE(cpu->get_carry());
  expectCycles(4, cpu);
}

TEST(CpuTest, ccf) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x3f));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0x3f));

  Cpu* cpu = new Cpu(&bus);
  cpu->tick();
  EXPECT_TRUE(cpu->get_carry());
  expectCycles(4, cpu);
  cpu->tick();
  EXPECT_FALSE(cpu->get_carry());
  expectCycles(4, cpu);
}

TEST(CpuTest, load_e_h) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b01011100));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_e(0xff);
  cpu->set_h(0x11);
  cpu->tick();
  EXPECT_EQ(0x11, cpu->get_e());
  expectCycles(4, cpu);
}

TEST(CpuTest, add_a_l) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b10000101));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_a(0xee);
  cpu->set_l(0x11);
  cpu->tick();
  EXPECT_EQ(0xff, cpu->get_a());
  expectCycles(4, cpu);
}

TEST(CpuTest, addc_a_r) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b10001000));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_a(0xee);
  cpu->set_b(0x10);
  cpu->set_carry(true);
  cpu->tick();
  EXPECT_EQ(0xff, cpu->get_a());
  expectCycles(4, cpu);
}

TEST(CpuTest, sub_a_r) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b10010000));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_a(0xee);
  cpu->set_b(0x11);
  cpu->tick();
  EXPECT_EQ(0xdd, cpu->get_a());
  expectCycles(4, cpu);
}

TEST(CpuTest, subc_a_r) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b10011000));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_a(0xee);
  cpu->set_b(0x10);
  cpu->set_carry(true);
  cpu->tick();
  EXPECT_EQ(0xdd, cpu->get_a());
  expectCycles(4, cpu);
}

TEST(CpuTest, and_a_r) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b10100000));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_a(0b10101010);
  cpu->set_b(0b01010101);
  cpu->tick();
  EXPECT_EQ(0, cpu->get_a());
  expectCycles(4, cpu);
}

TEST(CpuTest, xor_a_r) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b10101000));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_a(0b10101010);
  cpu->set_b(0b01010101);
  cpu->tick();
  EXPECT_EQ(0xff, cpu->get_a());
  expectCycles(4, cpu);
}

TEST(CpuTest, or_a_r) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b10110000));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_a(0b10111011);
  cpu->set_b(0b01010101);
  cpu->tick();
  EXPECT_EQ(0xff, cpu->get_a());
  expectCycles(4, cpu);
}

TEST(CpuTest, cp_a_r) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b10111000));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_a(1);
  cpu->set_b(1);
  cpu->tick();
  EXPECT_EQ(1, cpu->get_a());
  expectCycles(4, cpu);
}

TEST(CpuTest, ret_cc) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b11011000));
  EXPECT_CALL(bus, read(0xfe)).WillOnce(testing::Return(0x34));
  EXPECT_CALL(bus, read(0xff)).WillOnce(testing::Return(0x12));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_sp(0xfe);
  cpu->set_carry(true);
  cpu->tick();
  EXPECT_EQ(0x1234, cpu->get_pc());
  expectCycles(8, cpu);
}

TEST(CpuTest, pop) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b11000001));
  EXPECT_CALL(bus, read(0xfe)).WillOnce(testing::Return(0x34));
  EXPECT_CALL(bus, read(0xff)).WillOnce(testing::Return(0x12));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_sp(0xfe);
  cpu->tick();
  EXPECT_EQ(0x1234, cpu->get_bc());
  expectCycles(12, cpu);
}

TEST(CpuTest, pop_af) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b11110001));
  EXPECT_CALL(bus, read(0xfe)).WillOnce(testing::Return(0x34));
  EXPECT_CALL(bus, read(0xff)).WillOnce(testing::Return(0x12));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_sp(0xfe);
  cpu->tick();
  EXPECT_EQ(0x1234, cpu->get_af());
  expectCycles(12, cpu);
}

TEST(CpuTest, jp_cc_n16) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b11010010));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0x34));
  EXPECT_CALL(bus, read(2)).WillOnce(testing::Return(0x12));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_z(true);
  cpu->tick();
  EXPECT_EQ(0x1234, cpu->get_pc());
  expectCycles(12, cpu);
}

TEST(CpuTest, jp_n16) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b11000011));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0x34));
  EXPECT_CALL(bus, read(2)).WillOnce(testing::Return(0x12));

  Cpu* cpu = new Cpu(&bus);
  cpu->tick();
  EXPECT_EQ(0x1234, cpu->get_pc());
  expectCycles(12, cpu);
}

TEST(CpuTest, call_cc_n16) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b11001100));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0x34));
  EXPECT_CALL(bus, read(2)).WillOnce(testing::Return(0x12));
  EXPECT_CALL(bus, write(0xfffc, 3)).Times(1);
  EXPECT_CALL(bus, write(0xfffd, 0)).Times(1);

  Cpu* cpu = new Cpu(&bus);
  cpu->set_sp(0xfffe);
  cpu->set_z(true);
  cpu->tick();
  EXPECT_EQ(0x1234, cpu->get_pc());
  expectCycles(12, cpu);
}

TEST(CpuTest, push) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b11010101));
  EXPECT_CALL(bus, write(0xfffc, 0x34)).Times(1);
  EXPECT_CALL(bus, write(0xfffd, 0x12)).Times(1);

  Cpu* cpu = new Cpu(&bus);
  cpu->set_sp(0xfffe);
  cpu->set_de(0x1234);
  cpu->tick();
  expectCycles(16, cpu);
}

TEST(CpuTest, push_hl) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b11100101));
  EXPECT_CALL(bus, write(0xfffc, 0x34)).Times(1);
  EXPECT_CALL(bus, write(0xfffd, 0x12)).Times(1);

  Cpu* cpu = new Cpu(&bus);
  cpu->set_sp(0xfffe);
  cpu->set_hl(0x1234);
  cpu->tick();
  expectCycles(16, cpu);
}

TEST(CpuTest, add_a_n) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b11000110));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(1));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_a(0xfe);
  cpu->tick();
  EXPECT_EQ(0xff, cpu->get_a());
  expectCycles(8, cpu);
}

TEST(CpuTest, rst_n) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b11001111));
  EXPECT_CALL(bus, write(0xfffc, 1)).Times(1);
  EXPECT_CALL(bus, write(0xfffd, 0)).Times(1);

  Cpu* cpu = new Cpu(&bus);
  cpu->set_sp(0xfffe);
  cpu->tick();
  EXPECT_EQ(0x08, cpu->get_pc());
  expectCycles(32, cpu);
}

TEST(CpuTest, ret) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b11001001));
  EXPECT_CALL(bus, read(0xfffe)).WillOnce(testing::Return(0x34));
  EXPECT_CALL(bus, read(0xffff)).WillOnce(testing::Return(0x12));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_sp(0xfffe);
  cpu->tick();
  EXPECT_EQ(0x1234, cpu->get_pc());
  expectCycles(8, cpu);
}

TEST(CpuTest, call_n16) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b11001101));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0x34));
  EXPECT_CALL(bus, read(2)).WillOnce(testing::Return(0x12));
  EXPECT_CALL(bus, write(0xfffc, 3)).Times(1);
  EXPECT_CALL(bus, write(0xfffd, 0)).Times(1);

  Cpu* cpu = new Cpu(&bus);
  cpu->set_sp(0xfffe);
  cpu->tick();
  EXPECT_EQ(0x1234, cpu->get_pc());
  expectCycles(12, cpu);
}

TEST(CpuTest, addc_a_n) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b11001110));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0xfe));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_carry(true);
  cpu->tick();
  EXPECT_EQ(0xff, cpu->get_a());
  expectCycles(8, cpu);
}

TEST(CpuTest, sub_a_n) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b11010110));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0xfe));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_a(0xff);
  cpu->tick();
  EXPECT_EQ(1, cpu->get_a());
  expectCycles(8, cpu);
}

TEST(CpuTest, reti) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b11011001));
  EXPECT_CALL(bus, read(0xfffe)).WillOnce(testing::Return(0x34));
  EXPECT_CALL(bus, read(0xffff)).WillOnce(testing::Return(0x12));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_sp(0xfffe);
  cpu->tick();
  EXPECT_EQ(0x1234, cpu->get_pc());
  EXPECT_TRUE(cpu->get_ime());
  expectCycles(8, cpu);
}

TEST(CpuTest, subc_a_n) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b11011110));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0xfe));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_a(0xff);
  cpu->set_carry(true);
  cpu->tick();
  EXPECT_EQ(0, cpu->get_a());
  expectCycles(8, cpu);
}

TEST(CpuTest, load_n_a) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b11100000));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0xff));
  EXPECT_CALL(bus, write(0xffff, 0x11)).Times(1);

  Cpu* cpu = new Cpu(&bus);
  cpu->set_a(0x11);
  cpu->tick();
  expectCycles(12, cpu);
}

TEST(CpuTest, load_c_a) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b11100010));
  EXPECT_CALL(bus, write(0xffff, 0x11)).Times(1);

  Cpu* cpu = new Cpu(&bus);
  cpu->set_a(0x11);
  cpu->set_c(0xff);
  cpu->tick();
  expectCycles(8, cpu);
}

TEST(CpuTest, and_a_n) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b11100110));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0b11111110));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_a(0b00010001);
  cpu->tick();
  EXPECT_EQ(0b00010000, cpu->get_a());
  expectCycles(8, cpu);
}

TEST(CpuTest, add_sp_n) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b11101000));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0xff));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_sp(0xff00);
  cpu->tick();
  EXPECT_EQ(0xffff, cpu->get_sp());
  expectCycles(16, cpu);
}

TEST(CpuTest, jp_hl) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b11101001));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_pc(0);
  cpu->set_hl(0xffff);
  cpu->tick();
  EXPECT_EQ(0xffff, cpu->get_pc());
  expectCycles(4, cpu);
}

TEST(CpuTest, load_n16_a) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b11101010));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0x34));
  EXPECT_CALL(bus, read(2)).WillOnce(testing::Return(0x12));
  EXPECT_CALL(bus, write(0x1234, 0xff)).Times(1);

  Cpu* cpu = new Cpu(&bus);
  cpu->set_a(0xff);
  cpu->tick();
  expectCycles(16, cpu);
}

TEST(CpuTest, xor_a_n) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b11101110));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0b10101010));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_a(0b01010101);
  cpu->tick();
  EXPECT_EQ(0xff, cpu->get_a());
  expectCycles(8, cpu);
}

TEST(CpuTest, load_a_n) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b11110000));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0xff));
  EXPECT_CALL(bus, read(0xffff)).WillOnce(testing::Return(0x11));

  Cpu* cpu = new Cpu(&bus);
  cpu->tick();
  EXPECT_EQ(0x11, cpu->get_a());
  expectCycles(12, cpu);
}

TEST(CpuTest, load_a_c) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b11110010));
  EXPECT_CALL(bus, read(0xffff)).WillOnce(testing::Return(0x11));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_c(0xff);
  cpu->tick();
  EXPECT_EQ(0x11, cpu->get_a());
  expectCycles(8, cpu);
}

TEST(CpuTest, di) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b11110011));

  Cpu* cpu = new Cpu(&bus);
  cpu->tick();
  EXPECT_FALSE(cpu->get_ime());
  expectCycles(4, cpu);
}

TEST(CpuTest, or_a_n) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b11110110));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0xf0));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_a(0xf);
  cpu->tick();
  EXPECT_EQ(0xff, cpu->get_a());
  expectCycles(8, cpu);
}

TEST(CpuTest, load_hl_sp_n8) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b11111000));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0xff));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_sp(0xff00);
  cpu->set_hl(0);
  cpu->tick();
  EXPECT_EQ(0xffff, cpu->get_hl());
  expectCycles(12, cpu);
}

TEST(CpuTest, load_sp_hl) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b11111001));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_hl(0xff00);
  cpu->set_sp(0);
  cpu->tick();
  EXPECT_EQ(0xff00, cpu->get_sp());
  expectCycles(8, cpu);
}

TEST(CpuTest, load_a_n16) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b11111010));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0x34));
  EXPECT_CALL(bus, read(2)).WillOnce(testing::Return(0x12));
  EXPECT_CALL(bus, read(0x1234)).WillOnce(testing::Return(0xff));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_a(0);
  cpu->tick();
  EXPECT_EQ(0xff, cpu->get_a());
  expectCycles(16, cpu);
}

TEST(CpuTest, ei) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0b11111011));

  Cpu* cpu = new Cpu(&bus);
  cpu->tick();
  EXPECT_TRUE(cpu->get_ime());
  expectCycles(4, cpu);
}

TEST(CpuTest, rlc_n) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0xcb));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0b00000111));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_a(0b11001110);
  cpu->set_carry(false);
  cpu->tick();
  EXPECT_EQ(0b10011101, cpu->get_a());
  expectCycles(8, cpu);
}

TEST(CpuTest, rrc_n) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0xcb));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0b00001000));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_b(0b01001111);
  cpu->set_carry(false);
  cpu->tick();
  EXPECT_EQ(0b10100111, cpu->get_b());
  expectCycles(8, cpu);
}

TEST(CpuTest, rl_n) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0xcb));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0b00010000));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_b(0b01001111);
  cpu->set_carry(true);
  cpu->tick();
  EXPECT_EQ(0b10011111, cpu->get_b());
  expectCycles(8, cpu);
}

TEST(CpuTest, rr_n) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0xcb));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0b00011000));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_b(0b01001110);
  cpu->set_carry(true);
  cpu->tick();
  EXPECT_EQ(0b10100111, cpu->get_b());
  expectCycles(8, cpu);
}

TEST(CpuTest, sla_n) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0xcb));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0b00100000));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_b(0b11001110);
  cpu->set_carry(true);
  cpu->tick();
  EXPECT_EQ(0b10011100, cpu->get_b());
  expectCycles(8, cpu);
}

TEST(CpuTest, sra_n) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0xcb));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0b00101000));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_b(0b11001111);
  cpu->set_carry(true);
  cpu->tick();
  EXPECT_EQ(0b11100111, cpu->get_b());
  expectCycles(8, cpu);
}

TEST(CpuTest, swap) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0xcb));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0b00110000));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_b(0b11110000);
  cpu->tick();
  EXPECT_EQ(0b00001111, cpu->get_b());
  expectCycles(8, cpu);
}

TEST(CpuTest, srl_n) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0xcb));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0b00111000));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_b(0b11001111);
  cpu->set_carry(true);
  cpu->tick();
  EXPECT_EQ(0b01100111, cpu->get_b());
  expectCycles(8, cpu);
}

TEST(CpuTest, bit_b_r) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0xcb));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0b01100000));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_b(0b11101111);
  cpu->tick();
  EXPECT_EQ(true, cpu->get_z());
  expectCycles(8, cpu);
}

TEST(CpuTest, res_b_r) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0xcb));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0b10100000));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_b(0b11111111);
  cpu->tick();
  EXPECT_EQ(0b11101111, cpu->get_b());
  expectCycles(8, cpu);
}

TEST(CpuTest, set_b_r) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0xcb));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0b11100000));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_b(0);
  cpu->tick();
  EXPECT_EQ(0b00010000, cpu->get_b());
  expectCycles(8, cpu);
}

TEST(CpuTest, set_b_hl) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0xcb));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0b11100110));
  EXPECT_CALL(bus, read(0x1234)).WillOnce(testing::Return(0));
  EXPECT_CALL(bus, write(0x1234, 0b00010000)).Times(1);

  Cpu* cpu = new Cpu(&bus);
  cpu->set_hl(0x1234);
  cpu->tick();
  expectCycles(16, cpu);
}

}  // namespace gbemu
