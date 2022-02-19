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

void expectStallForNCycles(u8 n, Cpu* cpu) {
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
  expectStallForNCycles(4, cpu);
}

TEST(CpuTest, load_bc_n16) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x01));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0x34));
  EXPECT_CALL(bus, read(2)).WillOnce(testing::Return(0x12));

  Cpu* cpu = new Cpu(&bus);
  cpu->tick();
  EXPECT_EQ(0x1234, cpu->get_bc());
  expectStallForNCycles(12, cpu);
}

TEST(CpuTest, load_de_n16) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x11));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0x34));
  EXPECT_CALL(bus, read(2)).WillOnce(testing::Return(0x12));

  Cpu* cpu = new Cpu(&bus);
  cpu->tick();
  EXPECT_EQ(0x1234, cpu->get_de());
  expectStallForNCycles(12, cpu);
}

TEST(CpuTest, load_hl_n16) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x21));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0x34));
  EXPECT_CALL(bus, read(2)).WillOnce(testing::Return(0x12));

  Cpu* cpu = new Cpu(&bus);
  cpu->tick();
  EXPECT_EQ(0x1234, cpu->get_hl());
  expectStallForNCycles(12, cpu);
}

TEST(CpuTest, load_sp_n16) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x31));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0x34));
  EXPECT_CALL(bus, read(2)).WillOnce(testing::Return(0x12));

  Cpu* cpu = new Cpu(&bus);
  cpu->tick();
  EXPECT_EQ(0x1234, cpu->get_sp());
  expectStallForNCycles(12, cpu);
}

TEST(CpuTest, load_bc_a) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x02));
  EXPECT_CALL(bus, write(0xffff, 0x11)).Times(1);

  Cpu* cpu = new Cpu(&bus);
  cpu->set_a(0x11);
  cpu->set_bc(0xffff);
  cpu->tick();
  expectStallForNCycles(8, cpu);
}

TEST(CpuTest, load_de_a) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x12));
  EXPECT_CALL(bus, write(0xffff, 0x11)).Times(1);

  Cpu* cpu = new Cpu(&bus);
  cpu->set_a(0x11);
  cpu->set_de(0xffff);
  cpu->tick();
  expectStallForNCycles(8, cpu);
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
  expectStallForNCycles(8, cpu);
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
  expectStallForNCycles(8, cpu);
}

TEST(CpuTest, inc_r16) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x03));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_bc(0xfffe);
  cpu->tick();
  EXPECT_EQ(0xffff, cpu->get_bc());
  expectStallForNCycles(4, cpu);
}

TEST(CpuTest, inc_r8) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x04));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_b(0xfe);
  cpu->tick();
  EXPECT_EQ(0xff, cpu->get_b());
  expectStallForNCycles(4, cpu);
}

TEST(CpuTest, dec_r8) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x15));

  Cpu* cpu = new Cpu(&bus);
  cpu->set_d(0xff);
  cpu->tick();
  EXPECT_EQ(0xfe, cpu->get_d());
  expectStallForNCycles(4, cpu);
}

TEST(CpuTest, load_r_n8) {
  MockBus bus;
  EXPECT_CALL(bus, read(0)).WillOnce(testing::Return(0x0e));
  EXPECT_CALL(bus, read(1)).WillOnce(testing::Return(0xff));

  Cpu* cpu = new Cpu(&bus);
  cpu->tick();
  EXPECT_EQ(0xff, cpu->get_c());
  expectStallForNCycles(8, cpu);
}

}  // namespace gbemu
