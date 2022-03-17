#include "core/graphics/ppu.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "core/display/display.h"
#include "core/graphics/color.h"
#include "core/graphics/mode.h"
#include "core/interrupt/interrupt_controller_impl.h"
#include "core/memory/ram.h"
#include "core/types/types.h"

namespace gbeml {

class MockDisplay : public Display {
 public:
  MOCK_METHOD3(render, void(u8 x, u8 y, Color pixel));
  MOCK_METHOD0(getBuffer, u32*());
};

class MockVRam : public Ram {
 public:
  MOCK_METHOD2(write, void(u16 addr, u8 value));
  MOCK_CONST_METHOD1(read, u8(u16 addr));
};

class MockOam : public Ram {
 public:
  MOCK_METHOD2(write, void(u16 addr, u8 value));
  MOCK_CONST_METHOD1(read, u8(u16 addr));
};

TEST(PpuTest, moveNext) {
  MockDisplay display;
  MockVRam vram;
  MockOam oam;
  InterruptControllerImpl ic;
  Ppu ppu(&display, &vram, &oam, &ic);

  EXPECT_CALL(display, render(testing::_, testing::_, testing::_))
      .Times(testing::AnyNumber());
  EXPECT_CALL(vram, read(testing::_)).Times(testing::AnyNumber());
  EXPECT_CALL(oam, read(testing::_)).Times(testing::AnyNumber());

  ppu.writeLy(0);
  ppu.writeLcdc(0b10000001);
  ppu.init();

  EXPECT_EQ(PpuMode::OamScan, ppu.getMode());
  for (u64 ly = 0; ly < 154; ly++) {
    for (u64 lx = 0; lx < 456; lx++) {
      if (ly < 144) {
        EXPECT_EQ(ly, ppu.readLy());
        if (lx < 80) {
          EXPECT_EQ(PpuMode::OamScan, ppu.getMode());
        } else if (lx < 252) {
          EXPECT_EQ(PpuMode::Drawing, ppu.getMode());
        } else {
          EXPECT_EQ(PpuMode::HBlank, ppu.getMode());
        }
      } else {
        EXPECT_EQ(PpuMode::VBlank, ppu.getMode());
      }
      ppu.tick();
    }
  }
  EXPECT_EQ(PpuMode::OamScan, ppu.getMode());
}

TEST(PpuTest, drawWindow) {
  MockDisplay display;
  MockVRam vram;
  MockOam oam;
  InterruptControllerImpl ic;
  Ppu ppu(&display, &vram, &oam, &ic);

  EXPECT_CALL(display, render(0, 0, Color::White)).Times(1);
  EXPECT_CALL(vram, read(testing::_)).Times(testing::AnyNumber());
  EXPECT_CALL(oam, read(testing::_)).Times(testing::AnyNumber());

  ppu.writeLy(0);
  ppu.writeLcdc(0b10100001);
  ppu.writeWy(0);
  ppu.writeWx(7);
  ppu.init();

  EXPECT_EQ(PpuMode::OamScan, ppu.getMode());
  for (u64 ly = 0; ly < 80; ly++) {
    EXPECT_EQ(PpuMode::OamScan, ppu.getMode());
    ppu.tick();
  }
  for (u64 ly = 0; ly < 18; ly++) {
    EXPECT_EQ(PpuMode::Drawing, ppu.getMode());
    ppu.tick();
  }
  ppu.tick();
}

TEST(PpuTest, drawSprite) {
  MockDisplay display;
  MockVRam vram;
  MockOam oam;
  InterruptControllerImpl ic;
  Ppu ppu(&display, &vram, &oam, &ic);

  EXPECT_CALL(display, render(0, 0, Color::Black)).Times(1);
  EXPECT_CALL(vram, read(0)).WillOnce(testing::Return(0b10000000));
  EXPECT_CALL(vram, read(1)).WillOnce(testing::Return(0b10000000));
  EXPECT_CALL(vram, read(testing::Ge(2))).Times(testing::AnyNumber());
  EXPECT_CALL(oam, read(0)).WillOnce(testing::Return(16));
  EXPECT_CALL(oam, read(1)).WillOnce(testing::Return(8));
  EXPECT_CALL(oam, read(2)).WillOnce(testing::Return(0));
  EXPECT_CALL(oam, read(3)).WillOnce(testing::Return(0));
  EXPECT_CALL(oam, read(testing::Ge(4))).Times(testing::AnyNumber());

  ppu.writeLy(0);
  ppu.writeLcdc(0b10000011);
  ppu.writeObp0(0b11000000);
  ppu.init();

  EXPECT_EQ(PpuMode::OamScan, ppu.getMode());
  for (u64 ly = 0; ly < 80; ly++) {
    EXPECT_EQ(PpuMode::OamScan, ppu.getMode());
    ppu.tick();
  }
  for (u64 ly = 0; ly < 23; ly++) {
    EXPECT_EQ(PpuMode::Drawing, ppu.getMode());
    ppu.tick();
  }
  ppu.tick();
}

}  // namespace gbeml
