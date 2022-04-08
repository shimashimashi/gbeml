#include "core/timer/timer_impl.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "core/interrupt/interrupt_controller.h"

namespace gbeml {

class MockInterruptController : public InterruptController {
 public:
  MOCK_CONST_METHOD0(readInterruptFlag, u8());
  MOCK_CONST_METHOD0(readInterruptEnable, u8());

  MOCK_METHOD1(writeInterruptFlag, void(u8 value));
  MOCK_METHOD1(writeInterruptEnable, void(u8 value));

  MOCK_METHOD0(signalVBlank, void());
  MOCK_METHOD0(signalLcdStat, void());
  MOCK_METHOD0(signalTimer, void());
  MOCK_METHOD0(signalSerial, void());
  MOCK_METHOD0(signalJoypad, void());

  MOCK_METHOD0(clearVBlank, void());
  MOCK_METHOD0(clearLcdStat, void());
  MOCK_METHOD0(clearTimer, void());
  MOCK_METHOD0(clearSerial, void());
  MOCK_METHOD0(clearJoypad, void());

  MOCK_METHOD0(isVBlankRequested, bool());
  MOCK_METHOD0(isLcdStatRequested, bool());
  MOCK_METHOD0(isTimerRequested, bool());
  MOCK_METHOD0(isSerialRequested, bool());
  MOCK_METHOD0(isJoypadRequested, bool());
  MOCK_METHOD0(isInterruptRequested, bool());
};

TEST(TimerImplTest, tickDivider) {
  MockInterruptController ic;
  EXPECT_CALL(ic, signalTimer()).Times(testing::AnyNumber());

  TimerImpl timer(&ic, 0, 0, 0, 0);

  for (int i = 0; i < 256; ++i) {
    EXPECT_EQ(0, timer.readDivider());
    timer.tick();
  }
  EXPECT_EQ(1, timer.readDivider());

  timer.resetDivider();
  EXPECT_EQ(0, timer.readDivider());
}

TEST(TimerImplTest, tickCounter1024) {
  MockInterruptController ic;
  EXPECT_CALL(ic, signalTimer()).Times(testing::AnyNumber());

  TimerImpl timer(&ic, 0, 0, 0, 0b00000100);

  for (int i = 0; i < 1024; ++i) {
    EXPECT_EQ(0, timer.readCounter());
    timer.tick();
  }
  EXPECT_EQ(1, timer.readCounter());
}

TEST(TimerImplTest, tickCounter16) {
  MockInterruptController ic;
  EXPECT_CALL(ic, signalTimer()).Times(testing::AnyNumber());

  TimerImpl timer(&ic, 0, 0, 0, 0b00000101);

  EXPECT_EQ(0, timer.readCounter());
  for (int i = 0; i < 16; ++i) {
    EXPECT_EQ(0, timer.readCounter());
    timer.tick();
  }
  EXPECT_EQ(1, timer.readCounter());
}

TEST(TimerImplTest, tickCounter64) {
  MockInterruptController ic;
  EXPECT_CALL(ic, signalTimer()).Times(testing::AnyNumber());

  TimerImpl timer(&ic, 0, 0, 0, 0b00000110);

  for (int i = 0; i < 64; ++i) {
    EXPECT_EQ(0, timer.readCounter());
    timer.tick();
  }
  EXPECT_EQ(1, timer.readCounter());
}

TEST(TimerImplTest, tickCounter256) {
  MockInterruptController ic;
  EXPECT_CALL(ic, signalTimer()).Times(testing::AnyNumber());

  TimerImpl timer(&ic, 0, 0, 0, 0b00000111);

  for (int i = 0; i < 256; ++i) {
    EXPECT_EQ(0, timer.readCounter());
    timer.tick();
  }
  EXPECT_EQ(1, timer.readCounter());
}

TEST(TimerImplTest, tickCounterDisabled) {
  MockInterruptController ic;
  EXPECT_CALL(ic, signalTimer()).Times(testing::AnyNumber());

  TimerImpl timer(&ic, 0, 0, 0, 0b00000011);

  for (int i = 0; i < 256; ++i) {
    EXPECT_EQ(0, timer.readCounter());
    timer.tick();
  }
  EXPECT_EQ(0, timer.readCounter());
}

TEST(TimerImplTest, tickCounterModulo) {
  MockInterruptController ic;
  EXPECT_CALL(ic, signalTimer()).Times(1);

  TimerImpl timer(&ic, 0, 0, 0, 0);
  timer.writeCounter(0xff);
  timer.writeControl(0b00000111);
  timer.writeModulo(0xfe);

  EXPECT_EQ(0xff, timer.readCounter());
  EXPECT_EQ(0b00000111, timer.readControl());
  EXPECT_EQ(0xfe, timer.readModulo());

  for (int i = 0; i < 256; ++i) {
    EXPECT_EQ(0xff, timer.readCounter());
    timer.tick();
  }
  EXPECT_EQ(0xfe, timer.readCounter());
}

}  // namespace gbeml
