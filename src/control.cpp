#include <SDL_keycode.h>

#include "control.h"

void
Control::update(const SDL_Event& event)
{
  switch (event.type) {
    case SDL_KEYDOWN: {
      if (event.key.keysym.sym == SDLK_a) {
        this->left = true;
      }
      if (event.key.keysym.sym == SDLK_d) {
        this->right = true;
      }
      if (event.key.keysym.sym == SDLK_w) {
        this->up = true;
      }
      if (event.key.keysym.sym == SDLK_s) {
        this->down = true;
      }
      break;
    }
    case SDL_KEYUP: {
      if (event.key.keysym.sym == SDLK_a) {
        this->left = false;
      }
      if (event.key.keysym.sym == SDLK_d) {
        this->right = false;
      }
      if (event.key.keysym.sym == SDLK_w) {
        this->up = false;
      }
      if (event.key.keysym.sym == SDLK_s) {
        this->down = false;
      }
      break;
    }
    case SDL_MOUSEBUTTONDOWN: {
      if (event.button.button == 1) {
        this->is_player_shooting = true;
        this->is_pointer_down = true;
      }
      break;
    }
    case SDL_MOUSEBUTTONUP: {
      if (event.button.button == 1) {
        this->is_player_shooting = false;
        this->is_pointer_down = false;
      }
      break;
    }
  }
}
