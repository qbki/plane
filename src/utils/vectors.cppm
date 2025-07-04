module;
#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

export module pln.utils.vectors;

namespace pln::utils::vectors {

export
bool
is_looking_right_xy(const glm::vec3& forward_vector,
                    const glm::vec3& sight_vector)
{
  auto left_vector = glm::vec3(-forward_vector.y, forward_vector.x, 0.0);
  return glm::dot(left_vector, sight_vector) < 0;
}

}
