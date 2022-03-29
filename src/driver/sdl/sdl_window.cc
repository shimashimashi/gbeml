#include "driver/sdl/sdl_window.h"

#include "core/joypad/joypad.h"
#include "core/log/logging.h"
#include "core/types/types.h"

namespace gbeml {

bool SdlWindow::init() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    DCHECK(false);
    return false;
  }
  if (SDL_CreateWindowAndRenderer(width * 4, height * 4, 0, &window,
                                  &renderer) == -1) {
    DCHECK(false);
    return false;
  }
  surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
  return true;
}

void SdlWindow::runFrame() {
  for (int i = 0; i < 70224; ++i) {
    gb->tick();
  }

  if (SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);

  u32 *buffer = gb->getDisplay()->getBuffer();
  u32 *pixels = (u32 *)surface->pixels;
  for (int y = 0; y < 144; ++y) {
    for (int x = 0; x < 160; ++x) {
      pixels[x + y * width] = buffer[x + y * width];
    }
  }

  if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);

  SDL_Texture *screenTexture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_RenderClear(renderer);
  SDL_RenderCopyEx(renderer, screenTexture, NULL, NULL, NULL, NULL,
                   SDL_FLIP_NONE);
  SDL_RenderPresent(renderer);

  SDL_DestroyTexture(screenTexture);
}

void SdlWindow::runLoop(SDL_Event event) {
  switch (event.type) {
    case SDL_KEYDOWN:
      handleKeyDown(event.key);
      break;
    case SDL_KEYUP:
      handleKeyUp(event.key);
      break;
    default:
      break;
  }

  runFrame();
}

void SdlWindow::handleKeyDown(SDL_KeyboardEvent event) {
  switch (event.keysym.sym) {
    case SDLK_RETURN:
      gb->press(JoypadButton::Start);
      break;
    case SDLK_LSHIFT:
    case SDLK_RSHIFT:
      gb->press(JoypadButton::Select);
      break;
    case SDLK_x:
      gb->press(JoypadButton::A);
      break;
    case SDLK_z:
      gb->press(JoypadButton::B);
      break;
    case SDLK_UP:
      gb->press(JoypadButton::Up);
      break;
    case SDLK_DOWN:
      gb->press(JoypadButton::Down);
      break;
    case SDLK_LEFT:
      gb->press(JoypadButton::Left);
      break;
    case SDLK_RIGHT:
      gb->press(JoypadButton::Right);
      break;
    default:
      break;
  }
}

void SdlWindow::handleKeyUp(SDL_KeyboardEvent event) {
  switch (event.keysym.sym) {
    case SDLK_RETURN:
      gb->release(JoypadButton::Start);
      break;
    case SDLK_LSHIFT:
    case SDLK_RSHIFT:
      gb->release(JoypadButton::Select);
      break;
    case SDLK_x:
      gb->release(JoypadButton::A);
      break;
    case SDLK_z:
      gb->release(JoypadButton::B);
      break;
    case SDLK_UP:
      gb->release(JoypadButton::Up);
      break;
    case SDLK_DOWN:
      gb->release(JoypadButton::Down);
      break;
    case SDLK_LEFT:
      gb->release(JoypadButton::Left);
      break;
    case SDLK_RIGHT:
      gb->release(JoypadButton::Right);
      break;
    default:
      break;
  }
}

}  // namespace gbeml
