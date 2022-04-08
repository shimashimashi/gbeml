#include "core/joypad/joypad_impl.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "core/interrupt/interrupt_controller.h"
#include "core/joypad/joypad.h"

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

TEST(JoypadImplTest, pressAction) {
  MockInterruptController ic;
  EXPECT_CALL(ic, signalJoypad()).Times(testing::AnyNumber());

  JoypadImpl joypad_impl(&ic);

  joypad_impl.write(0b00000000);

  joypad_impl.press(JoypadButton::Start);
  EXPECT_EQ(0b11010111, joypad_impl.read());
  joypad_impl.release(JoypadButton::Start);
  EXPECT_EQ(0b11011111, joypad_impl.read());

  joypad_impl.press(JoypadButton::Select);
  EXPECT_EQ(0b11011011, joypad_impl.read());
  joypad_impl.release(JoypadButton::Select);
  EXPECT_EQ(0b11011111, joypad_impl.read());

  joypad_impl.press(JoypadButton::B);
  EXPECT_EQ(0b11011101, joypad_impl.read());
  joypad_impl.release(JoypadButton::B);
  EXPECT_EQ(0b11011111, joypad_impl.read());

  joypad_impl.press(JoypadButton::A);
  EXPECT_EQ(0b11011110, joypad_impl.read());
  joypad_impl.release(JoypadButton::A);
  EXPECT_EQ(0b11011111, joypad_impl.read());
}

TEST(JoypadImplTest, pressDirection) {
  MockInterruptController ic;
  EXPECT_CALL(ic, signalJoypad()).Times(testing::AnyNumber());

  JoypadImpl joypad_impl(&ic);

  joypad_impl.write(0b00100000);

  joypad_impl.press(JoypadButton::Down);
  EXPECT_EQ(0b11100111, joypad_impl.read());
  joypad_impl.release(JoypadButton::Down);
  EXPECT_EQ(0b11101111, joypad_impl.read());

  joypad_impl.press(JoypadButton::Up);
  EXPECT_EQ(0b11101011, joypad_impl.read());
  joypad_impl.release(JoypadButton::Up);
  EXPECT_EQ(0b11101111, joypad_impl.read());

  joypad_impl.press(JoypadButton::Left);
  EXPECT_EQ(0b11101101, joypad_impl.read());
  joypad_impl.release(JoypadButton::Left);
  EXPECT_EQ(0b11101111, joypad_impl.read());

  joypad_impl.press(JoypadButton::Right);
  EXPECT_EQ(0b11101110, joypad_impl.read());
  joypad_impl.release(JoypadButton::Right);
  EXPECT_EQ(0b11101111, joypad_impl.read());
}

}  // namespace gbeml
