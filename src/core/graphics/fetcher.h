#ifndef GBEML_FETCHER_H_
#define GBEML_FETCHER_H_

#include <vector>

#include "core/graphics/color.h"
#include "core/graphics/lcdc.h"
#include "core/graphics/palette.h"
#include "core/graphics/pixel.h"
#include "core/graphics/sprite.h"
#include "core/graphics/tile.h"
#include "core/memory/ram.h"
#include "core/types/types.h"

namespace gbeml {

class PixelFetcher {
 public:
  PixelFetcher(const Ram& vram_, const Lcdc& lcdc_,
               const BackgroundPalette& bgp_, const SpritePalette& obp0_,
               const SpritePalette& obp1_)
      : vram(vram_), lcdc(lcdc_), bgp(bgp_), obp0(obp0_), obp1(obp1_) {}

  std::vector<SpritePixel> fetchSpritePixels(
      u8 x, u8 y, const std::vector<Sprite>& visible_sprites) const;
  Tile fetchBackgroundPixels(u8 scx, u8 scy, u8 ly);
  Tile fetchWindowPixels(u8 ly);

  void reset();

 private:
  const Ram& vram;
  const Lcdc& lcdc;
  const BackgroundPalette& bgp;
  const SpritePalette& obp0;
  const SpritePalette& obp1;

  u8 fetcher_x = 0;

  u8 getBackgroundTileNumber(u8 scx, u8 scy, u8 ly) const;
  u16 getBackgroundTileDataAddress(u8 tile_number, u8 scy, u8 ly) const;

  u8 getWindowTileNumber(u8 window_y) const;
  u16 getWindowTileDataAddress(u8 tile_number, u8 window_y) const;
};

}  // namespace gbeml

#endif  // GBEML_FETCHER_H_
