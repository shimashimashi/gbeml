#include "core/graphics/fetcher.h"

namespace gbeml {

std::vector<SpritePixel> PixelFetcher::fetchSpritePixels(
    u8 x, u8 y, const std::vector<Sprite>& visible_sprites) const {
  std::vector<SpritePixel> pixels(8, SpritePixel(Color::Transparent, true));

  for (const Sprite& sprite : visible_sprites) {
    u16 addr = sprite.getTileDataAddress(y, lcdc.spriteSize());
    u8 low = vram.read(addr);
    u8 high = vram.read(addr + 1);
    const SpritePalette& palette = sprite.getPaletteNumber() ? obp1 : obp0;

    Tile tile(low, high, palette);
    if (sprite.flipX()) {
      tile.reverse();
    }

    for (u8 i = 0; i < 8; ++i) {
      if (pixels[i].getColor() != Color::Transparent) {
        continue;
      }
      if (!sprite.isVisibleHorizontally(x + i)) {
        continue;
      }
      u8 index = sprite.getPixelIndex(x + i);
      Color color = tile.getAt(index);
      if (color != Color::Transparent) {
        pixels[i] = SpritePixel(color, sprite.isBackgroundOverSprite());
      }
    }
  }

  return pixels;
}

Tile PixelFetcher::fetchBackgroundPixels(u8 scx, u8 scy, u8 ly) {
  u8 tile_number = getBackgroundTileNumber(scx, scy, ly);
  u16 addr = getBackgroundTileDataAddress(tile_number, scy, ly);

  u8 low = vram.read(addr);
  u8 high = vram.read(addr + 1);

  fetcher_x += 8;
  Tile tile(low, high, bgp);
  return tile;
}

Tile PixelFetcher::fetchWindowPixels(u8 window_y) {
  u8 tile_number = getWindowTileNumber(window_y);
  u16 addr = getWindowTileDataAddress(tile_number, window_y);

  u8 low = vram.read(addr);
  u8 high = vram.read(addr + 1);

  fetcher_x += 8;
  Tile tile(low, high, bgp);
  return tile;
}

u8 PixelFetcher::getBackgroundTileNumber(u8 scx, u8 scy, u8 ly) const {
  u8 x = ((scx + fetcher_x) & 0xff) / 8;
  u8 y = ((scy + ly) & 0xff) / 8;
  u16 offset = (x + y * 32) & 0x3ff;
  u16 addr = lcdc.getBackgroundTileMapAddress(offset);
  return vram.read(addr);
}

u16 PixelFetcher::getBackgroundTileDataAddress(u8 tile_number, u8 scy,
                                               u8 ly) const {
  u16 base = lcdc.getBackgroundTileDataAddress(tile_number);
  u16 offset = 2 * ((ly + scy) % 8);
  return base + offset;
}

u8 PixelFetcher::getWindowTileNumber(u8 window_y) const {
  u8 x = (fetcher_x & 0xff) / 8;
  u8 y = (window_y & 0xff) / 8;
  u16 offset = (x + y * 32) & 0x3ff;
  u16 addr = lcdc.getWindowTileMapAddress(offset);
  return vram.read(addr);
}

u16 PixelFetcher::getWindowTileDataAddress(u8 tile_number, u8 window_y) const {
  u16 base = lcdc.getBackgroundTileDataAddress(tile_number);
  u16 offset = 2 * (window_y % 8);
  return base + offset;
}

void PixelFetcher::reset() { fetcher_x = 0; }

}  // namespace gbeml
