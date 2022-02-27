#include <MiniFB.h>

#include "core/types/types.h"

namespace gbemu {

#define kUnused(var) (void)var;
#define WIDTH 160
#define HEIGHT 140

class MiniFbEvents {
 public:
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
};

class MiniFbWindow {
 public:
  bool init();

  bool waitSync();

  bool update(u32 *buffer);

 private:
  struct mfb_window *window;
};

}  // namespace gbemu
