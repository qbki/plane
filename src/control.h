#pragma once
#include <SDL2/SDL.h>

class Control {
public:
  bool up = false;
  bool down = false;
  bool left = false;
  bool right = false;

  void update(SDL_Event& event);
};
