#include "core/interrupt/interrupt_controller_impl.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace gbeml {

TEST(InterruptControllerImplTest, readWrite) {
  InterruptControllerImpl ic;

  ic.writeInterruptFlag(0xff);
  EXPECT_EQ(0xff, ic.readInterruptFlag());

  ic.writeInterruptEnable(0xff);
  EXPECT_EQ(0xff, ic.readInterruptEnable());
}

TEST(InterruptControllerImplTest, isTimerRequested) {
  InterruptControllerImpl ic;

  EXPECT_EQ(false, ic.isTimerRequested());
  ic.signalTimer();
  EXPECT_EQ(false, ic.isTimerRequested());
  ic.writeInterruptEnable(0x04);
  EXPECT_EQ(true, ic.isTimerRequested());
}

TEST(InterruptControllerImplTest, isVBlankRequested) {
  InterruptControllerImpl ic;

  EXPECT_EQ(false, ic.isVBlankRequested());
  ic.signalVBlank();
  EXPECT_EQ(false, ic.isVBlankRequested());
  ic.writeInterruptEnable(0x01);
  EXPECT_EQ(true, ic.isVBlankRequested());
}

TEST(InterruptControllerImplTest, isLcdStatRequested) {
  InterruptControllerImpl ic;

  EXPECT_EQ(false, ic.isLcdStatRequested());
  ic.signalLcdStat();
  EXPECT_EQ(false, ic.isLcdStatRequested());
  ic.writeInterruptEnable(0x02);
  EXPECT_EQ(true, ic.isLcdStatRequested());
}

TEST(InterruptControllerImplTest, isSerialRequested) {
  InterruptControllerImpl ic;

  EXPECT_EQ(false, ic.isSerialRequested());
  ic.signalSerial();
  EXPECT_EQ(false, ic.isSerialRequested());
  ic.writeInterruptEnable(0x08);
  EXPECT_EQ(true, ic.isSerialRequested());
}

TEST(InterruptControllerImplTest, isJoypadRequested) {
  InterruptControllerImpl ic;

  EXPECT_EQ(false, ic.isJoypadRequested());
  ic.signalJoypad();
  EXPECT_EQ(false, ic.isJoypadRequested());
  ic.writeInterruptEnable(0x10);
  EXPECT_EQ(true, ic.isJoypadRequested());
}

}  // namespace gbeml
