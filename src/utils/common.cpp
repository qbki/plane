#include "common.h"

glm::vec3
calc_z_direction(const glm::vec3& rotation)
{
  return { glm::cos(rotation.z), glm::sin(rotation.z), 0.0 };
}

glm::mat4
make_transform_matrix(glm::vec3 position, glm::vec3 rotation)
{
  auto rotation_matrix =
    glm::rotate(glm::mat4(1.0), rotation.z, { 0.0, 0.0, 1.0 });
  auto transform_matrix = glm::translate(glm::mat4(1.0), position);
  return transform_matrix * rotation_matrix;
}
