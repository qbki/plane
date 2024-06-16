#pragma once
#include <SDL_events.h>

class Control
{
public:
  enum class Action
  {
    UNKNOWN = 0,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    SHOOTING,
  };

  bool up = false;
  bool down = false;
  bool left = false;
  bool right = false;
  bool is_player_shooting = false;

  bool is_pointer_down = false;

  void update(const SDL_Event& event);
};
