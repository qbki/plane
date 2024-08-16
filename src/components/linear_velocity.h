#include <glm/gtc/constants.hpp>
#include <glm/vec3.hpp>

struct LinearVelocity
{
  float speed = 0;
  glm::vec3 velocity = glm::zero<glm::vec3>();

  LinearVelocity() = default;
  explicit LinearVelocity(float speed_value);
};
