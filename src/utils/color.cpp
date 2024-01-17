#include <glm/exponential.hpp>

#include "color.h"

glm::vec3
encode_gamma(const glm::vec3& color, float gamma)
{
  return glm::pow(color, glm::vec3(1.0f / gamma));
}

glm::vec3
decode_gamma(const glm::vec3& color, float gamma)
{
  return glm::pow(color, glm::vec3(gamma));
}
