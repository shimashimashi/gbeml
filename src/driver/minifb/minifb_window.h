#include <MiniFB.h>

#include "core/gameboy.h"
#include "core/types/types.h"

namespace gbeml {

#define kUnused(var) (void)var;
#define WIDTH 160
#define HEIGHT 144

class MiniFbEvents {
 public:
  MiniFbEvents(GameBoy *gb_) : gb(gb_){};

  void active(struct mfb_window *window, bool isActive);

  void resize(struct mfb_window *window, int width, int height);

  bool close(struct mfb_window *window);

  void keyboard(struct mfb_window *window, mfb_key key, mfb_key_mod mod,
                bool isPressed);

  void char_input(struct mfb_window *window, unsigned int charCode);

  void mouse_button(struct mfb_window *window, mfb_mouse_button button,
                    mfb_key_mod mod, bool isPressed);

  void mouse_move(struct mfb_window *window, int x, int y);

  void mouse_scroll(struct mfb_window *window, mfb_key_mod mod, float deltaX,
                    float deltaY);

 private:
  GameBoy *gb;
};

class MiniFbWindow {
 public:
  MiniFbWindow(GameBoy *gb_) : gb(gb_), events(gb_){};

  bool init();

  bool waitSync();

  bool update(u32 *buffer);

 private:
  GameBoy *gb;
  struct mfb_window *window;
  MiniFbEvents events;
};

}  // namespace gbeml
