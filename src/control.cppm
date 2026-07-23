module;
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_keycode.h>

export module pln.control;

namespace pln::control {

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
    case SDL_EVENT_KEY_DOWN: {
      if (event.key.key == SDLK_A) {
        this->left = true;
      }
      if (event.key.key == SDLK_D) {
        this->right = true;
      }
      if (event.key.key == SDLK_W) {
        this->up = true;
      }
      if (event.key.key == SDLK_S) {
        this->down = true;
      }
      if (event.key.key == SDLK_ESCAPE) {
        this->cancel = true;
      }
      break;
    }
    case SDL_EVENT_KEY_UP: {
      if (event.key.key == SDLK_A) {
        this->left = false;
      }
      if (event.key.key == SDLK_D) {
        this->right = false;
      }
      if (event.key.key == SDLK_W) {
        this->up = false;
      }
      if (event.key.key == SDLK_S) {
        this->down = false;
      }
      if (event.key.key == SDLK_ESCAPE) {
        this->cancel = false;
      }
      break;
    }
    case SDL_EVENT_MOUSE_BUTTON_DOWN: {
      if (event.button.button == 1) {
        this->shooting = true;
        this->pointer_pressed = true;
      }
      break;
    }
    case SDL_EVENT_MOUSE_BUTTON_UP: {
      if (event.button.button == 1) {
        this->shooting = false;
        this->pointer_pressed = false;
      }
      break;
    }
  }
}

}
