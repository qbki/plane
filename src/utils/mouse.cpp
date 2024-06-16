#include "mouse.h"

glm::ivec2
mouse_position()
{
  int x = 0;
  int y = 0;
  SDL_GetMouseState(&x, &y);
  return { x, y };
}
