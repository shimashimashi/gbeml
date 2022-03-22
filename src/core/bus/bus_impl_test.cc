#include "core/bus/bus_impl.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "core/display/display.h"
#include "core/graphics/ppu.h"
#include "core/interrupt/interrupt_controller.h"
#include "core/joypad/joypad.h"
#include "core/memory/mbc.h"
#include "core/memory/ram.h"
#include "core/timer/timer.h"

namespace gbeml {

class MockDisplay : public Display {
 public:
  MOCK_METHOD3(render, void(u8 x, u8 y, Color pixel));
  MOCK_METHOD0(getBuffer, u32*());
};

class MockMbc : public Mbc {
 public:
  MOCK_METHOD2(writeRom, void(u16 addr, u8 value));
  MOCK_METHOD2(writeRam, void(u16 addr, u8 value));
  MOCK_CONST_METHOD1(readRom, u8(u16 addr));
  MOCK_CONST_METHOD1(readRam, u8(u16 addr));
};

class MockRam : public Ram {
 public:
  MOCK_METHOD2(write, void(u16 addr, u8 value));
  MOCK_CONST_METHOD1(read, u8(u16 addr));
};

class MockTimer : public Timer {
 public:
  MOCK_CONST_METHOD0(readDivider, u8());
  MOCK_CONST_METHOD0(readCounter, u8());
  MOCK_CONST_METHOD0(readModulo, u8());
  MOCK_CONST_METHOD0(readControl, u8());

  MOCK_METHOD0(resetDivider, void());
  MOCK_METHOD1(writeCounter, void(u8 value));
  MOCK_METHOD1(writeModulo, void(u8 value));
  MOCK_METHOD1(writeControl, void(u8 value));

  MOCK_METHOD0(tick, void());
};

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

class MockJoypad : public Joypad {
 public:
  MOCK_CONST_METHOD0(read, u8());
  MOCK_METHOD1(write, void(u8 value));

  MOCK_METHOD1(press, void(JoypadButton button));
  MOCK_METHOD1(release, void(JoypadButton button));
};

class MockPpu : public Ppu {
 public:
  MOCK_METHOD0(tick, void());
  MOCK_METHOD0(init, void());

  MOCK_CONST_METHOD1(readVram, u8(u16 addr));
  MOCK_CONST_METHOD1(readOam, u8(u16 addr));

  MOCK_CONST_METHOD0(readLcdc, u8());
  MOCK_CONST_METHOD0(readLcdStat, u8());
  MOCK_CONST_METHOD0(readScy, u8());
  MOCK_CONST_METHOD0(readScx, u8());
  MOCK_CONST_METHOD0(readLy, u8());
  MOCK_CONST_METHOD0(readLyc, u8());
  MOCK_CONST_METHOD0(readWy, u8());
  MOCK_CONST_METHOD0(readWx, u8());
  MOCK_CONST_METHOD0(readBgp, u8());
  MOCK_CONST_METHOD0(readObp0, u8());
  MOCK_CONST_METHOD0(readObp1, u8());

  MOCK_METHOD2(writeVram, void(u16 addr, u8 value));
  MOCK_METHOD2(writeOam, void(u16 addr, u8 value));

  MOCK_METHOD1(writeLcdc, void(u8 value));
  MOCK_METHOD1(writeLcdStat, void(u8 value));
  MOCK_METHOD1(writeScy, void(u8 value));
  MOCK_METHOD1(writeScx, void(u8 value));
  MOCK_METHOD1(writeLy, void(u8 value));
  MOCK_METHOD1(writeLyc, void(u8 value));
  MOCK_METHOD1(writeWy, void(u8 value));
  MOCK_METHOD1(writeWx, void(u8 value));
  MOCK_METHOD1(writeBgp, void(u8 value));
  MOCK_METHOD1(writeObp0, void(u8 value));
  MOCK_METHOD1(writeObp1, void(u8 value));
};

TEST(BusImplTest, read) {
  MockDisplay display;
  MockRam hram;
  MockRam wram;
  MockMbc mbc;
  MockTimer timer;
  MockInterruptController ic;
  MockJoypad joypad;
  MockPpu ppu;

  BusImpl bus_impl(&mbc, &wram, &hram, &ppu, &timer, &ic, &joypad);

  EXPECT_CALL(mbc, readRom(0x0000)).WillOnce(testing::Return(1));
  EXPECT_CALL(mbc, readRom(0x7fff)).WillOnce(testing::Return(2));
  EXPECT_CALL(ppu, readVram(0x0000)).WillOnce(testing::Return(3));
  EXPECT_CALL(ppu, readVram(0x1fff)).WillOnce(testing::Return(4));
  EXPECT_CALL(mbc, readRam(0x0000)).WillOnce(testing::Return(5));
  EXPECT_CALL(mbc, readRam(0x1fff)).WillOnce(testing::Return(6));
  EXPECT_CALL(wram, read(0x0000))
      .WillOnce(testing::Return(7))
      .WillOnce(testing::Return(9));
  EXPECT_CALL(wram, read(0x1fff)).WillOnce(testing::Return(8));
  EXPECT_CALL(wram, read(0x1dff)).WillOnce(testing::Return(10));
  EXPECT_CALL(ppu, readOam(0x0000)).WillOnce(testing::Return(11));
  EXPECT_CALL(ppu, readOam(0x009f)).WillOnce(testing::Return(12));
  EXPECT_CALL(joypad, read()).WillOnce(testing::Return(13));
  EXPECT_CALL(timer, readDivider()).WillOnce(testing::Return(14));
  EXPECT_CALL(timer, readCounter()).WillOnce(testing::Return(15));
  EXPECT_CALL(timer, readModulo()).WillOnce(testing::Return(16));
  EXPECT_CALL(timer, readControl()).WillOnce(testing::Return(17));
  EXPECT_CALL(ic, readInterruptFlag()).WillOnce(testing::Return(18));
  EXPECT_CALL(ppu, readLcdc()).WillOnce(testing::Return(19));
  EXPECT_CALL(ppu, readLcdStat()).WillOnce(testing::Return(20));
  EXPECT_CALL(ppu, readScy()).WillOnce(testing::Return(21));
  EXPECT_CALL(ppu, readScx()).WillOnce(testing::Return(22));
  EXPECT_CALL(ppu, readLy()).WillOnce(testing::Return(23));
  EXPECT_CALL(ppu, readLyc()).WillOnce(testing::Return(24));
  EXPECT_CALL(ppu, readBgp()).WillOnce(testing::Return(25));
  EXPECT_CALL(ppu, readObp0()).WillOnce(testing::Return(26));
  EXPECT_CALL(ppu, readObp1()).WillOnce(testing::Return(27));
  EXPECT_CALL(ppu, readWy()).WillOnce(testing::Return(28));
  EXPECT_CALL(ppu, readWx()).WillOnce(testing::Return(29));
  EXPECT_CALL(hram, read(0x0000)).WillOnce(testing::Return(30));
  EXPECT_CALL(hram, read(0x007e)).WillOnce(testing::Return(31));
  EXPECT_CALL(ic, readInterruptEnable()).WillOnce(testing::Return(32));

  EXPECT_EQ(1, bus_impl.read(0x0000));
  EXPECT_EQ(2, bus_impl.read(0x7fff));
  EXPECT_EQ(3, bus_impl.read(0x8000));
  EXPECT_EQ(4, bus_impl.read(0x9fff));
  EXPECT_EQ(5, bus_impl.read(0xa000));
  EXPECT_EQ(6, bus_impl.read(0xbfff));
  EXPECT_EQ(7, bus_impl.read(0xc000));
  EXPECT_EQ(8, bus_impl.read(0xdfff));
  EXPECT_EQ(9, bus_impl.read(0xe000));
  EXPECT_EQ(10, bus_impl.read(0xfdff));
  EXPECT_EQ(11, bus_impl.read(0xfe00));
  EXPECT_EQ(12, bus_impl.read(0xfe9f));
  EXPECT_EQ(13, bus_impl.read(0xff00));
  EXPECT_EQ(14, bus_impl.read(0xff04));
  EXPECT_EQ(15, bus_impl.read(0xff05));
  EXPECT_EQ(16, bus_impl.read(0xff06));
  EXPECT_EQ(17, bus_impl.read(0xff07));
  EXPECT_EQ(18, bus_impl.read(0xff0f));
  EXPECT_EQ(19, bus_impl.read(0xff40));
  EXPECT_EQ(20, bus_impl.read(0xff41));
  EXPECT_EQ(21, bus_impl.read(0xff42));
  EXPECT_EQ(22, bus_impl.read(0xff43));
  EXPECT_EQ(23, bus_impl.read(0xff44));
  EXPECT_EQ(24, bus_impl.read(0xff45));
  EXPECT_EQ(25, bus_impl.read(0xff47));
  EXPECT_EQ(26, bus_impl.read(0xff48));
  EXPECT_EQ(27, bus_impl.read(0xff49));
  EXPECT_EQ(28, bus_impl.read(0xff4a));
  EXPECT_EQ(29, bus_impl.read(0xff4b));
  EXPECT_EQ(30, bus_impl.read(0xff80));
  EXPECT_EQ(31, bus_impl.read(0xfffe));
  EXPECT_EQ(32, bus_impl.read(0xffff));
}

TEST(BusImplTest, write) {
  MockDisplay display;
  MockRam hram;
  MockRam wram;
  MockMbc mbc;
  MockTimer timer;
  MockInterruptController ic;
  MockJoypad joypad;
  MockPpu ppu;

  BusImpl bus_impl(&mbc, &wram, &hram, &ppu, &timer, &ic, &joypad);

  EXPECT_CALL(mbc, writeRom(0x0000, 1)).Times(1);
  EXPECT_CALL(mbc, writeRom(0x7fff, 2)).Times(1);
  EXPECT_CALL(ppu, writeVram(0x0000, 3)).Times(1);
  EXPECT_CALL(ppu, writeVram(0x1fff, 4)).Times(1);
  EXPECT_CALL(mbc, writeRam(0x0000, 5)).Times(1);
  EXPECT_CALL(mbc, writeRam(0x1fff, 6)).Times(1);
  EXPECT_CALL(wram, write(0x0000, 7)).Times(1);
  EXPECT_CALL(wram, write(0x1fff, 8)).Times(1);
  EXPECT_CALL(wram, write(0x0000, 9)).Times(1);
  EXPECT_CALL(wram, write(0x1dff, 10)).Times(1);
  EXPECT_CALL(ppu, writeOam(0x0000, 11)).Times(1);
  EXPECT_CALL(ppu, writeOam(0x009f, 12)).Times(1);
  EXPECT_CALL(joypad, write(13)).Times(1);
  EXPECT_CALL(timer, resetDivider()).Times(1);
  EXPECT_CALL(timer, writeCounter(15)).Times(1);
  EXPECT_CALL(timer, writeModulo(16)).Times(1);
  EXPECT_CALL(timer, writeControl(17)).Times(1);
  EXPECT_CALL(ic, writeInterruptFlag(18)).Times(1);
  EXPECT_CALL(ppu, writeLcdc(19)).Times(1);
  EXPECT_CALL(ppu, writeLcdStat(20)).Times(1);
  EXPECT_CALL(ppu, writeScy(21)).Times(1);
  EXPECT_CALL(ppu, writeScx(22)).Times(1);
  EXPECT_CALL(ppu, writeLy(23)).Times(1);
  EXPECT_CALL(ppu, writeLyc(24)).Times(1);
  EXPECT_CALL(ppu, writeBgp(25)).Times(1);
  EXPECT_CALL(ppu, writeObp0(26)).Times(1);
  EXPECT_CALL(ppu, writeObp1(27)).Times(1);
  EXPECT_CALL(ppu, writeWy(28)).Times(1);
  EXPECT_CALL(ppu, writeWx(29)).Times(1);
  EXPECT_CALL(hram, write(0x0000, 30)).Times(1);
  EXPECT_CALL(hram, write(0x007e, 31)).Times(1);
  EXPECT_CALL(ic, writeInterruptEnable(32)).Times(1);

  bus_impl.write(0x0000, 1);
  bus_impl.write(0x7fff, 2);
  bus_impl.write(0x8000, 3);
  bus_impl.write(0x9fff, 4);
  bus_impl.write(0xa000, 5);
  bus_impl.write(0xbfff, 6);
  bus_impl.write(0xc000, 7);
  bus_impl.write(0xdfff, 8);
  bus_impl.write(0xe000, 9);
  bus_impl.write(0xfdff, 10);
  bus_impl.write(0xfe00, 11);
  bus_impl.write(0xfe9f, 12);
  bus_impl.write(0xff00, 13);
  bus_impl.write(0xff04, 14);
  bus_impl.write(0xff05, 15);
  bus_impl.write(0xff06, 16);
  bus_impl.write(0xff07, 17);
  bus_impl.write(0xff0f, 18);
  bus_impl.write(0xff40, 19);
  bus_impl.write(0xff41, 20);
  bus_impl.write(0xff42, 21);
  bus_impl.write(0xff43, 22);
  bus_impl.write(0xff44, 23);
  bus_impl.write(0xff45, 24);
  bus_impl.write(0xff47, 25);
  bus_impl.write(0xff48, 26);
  bus_impl.write(0xff49, 27);
  bus_impl.write(0xff4a, 28);
  bus_impl.write(0xff4b, 29);
  bus_impl.write(0xff80, 30);
  bus_impl.write(0xfffe, 31);
  bus_impl.write(0xffff, 32);
}

}  // namespace gbeml
