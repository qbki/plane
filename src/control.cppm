module;
#include <SDL_events.h>
#include <SDL_keycode.h>

export module control;

namespace control {

export enum class Action
{
  UNKNOWN = 0,
  UP,
  DOWN,
  LEFT,
  RIGHT,
  SHOOTING,
};

export class Control
{
public:
  bool up = false;
  bool down = false;
  bool left = false;
  bool right = false;
  bool shooting = false;
  bool cancel = false;
  bool pointer_pressed = false;

  void update(const SDL_Event& event);
};

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
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        this->cancel = true;
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
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        this->cancel = false;
      }
      break;
    }
    case SDL_MOUSEBUTTONDOWN: {
      if (event.button.button == 1) {
        this->shooting = true;
        this->pointer_pressed = true;
      }
      break;
    }
    case SDL_MOUSEBUTTONUP: {
      if (event.button.button == 1) {
        this->shooting = false;
        this->pointer_pressed = false;
      }
      break;
    }
  }
}

}
