#ifndef GBEML_SDL_WINDOW_H_
#define GBEML_SDL_WINDOW_H_

#include <SDL.h>

#include "core/gameboy.h"

namespace gbeml {

class SdlWindow {
 public:
  SdlWindow(GameBoy *gb_) : gb(gb_) {}

  bool init();
  void runFrame();
  void runLoop(SDL_Event event);

 private:
  GameBoy *gb;
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Surface *surface;

  const u64 width = 160;
  const u64 height = 144;

  void handleKeyDown(SDL_KeyboardEvent event);
  void handleKeyUp(SDL_KeyboardEvent event);
};

}  // namespace gbeml

#endif  // GBEML_SDL_WINDOW_H_
