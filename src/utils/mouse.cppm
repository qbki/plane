module;
#include <SDL_mouse.h>
#include <glm/vec2.hpp>

export module pln.utils.mouse;

namespace pln::utils::mouse {

export
glm::ivec2
mouse_position()
{
  int x = 0;
  int y = 0;
  SDL_GetMouseState(&x, &y);
  return { x, y };
}

}
