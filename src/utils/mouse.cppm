module;
#include <SDL3/SDL_mouse.h>
#include <glm/vec2.hpp>

export module pln.utils.mouse;

namespace pln::utils::mouse {

export
glm::vec2
mouse_position()
{
  float x = 0;
  float y = 0;
  SDL_GetMouseState(&x, &y);
  return { x, y };
}

}
