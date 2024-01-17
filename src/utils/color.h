#pragma once
#include <glm/vec3.hpp>

glm::vec3
encode_gamma(const glm::vec3& color, float gamma);

glm::vec3
decode_gamma(const glm::vec3& color, float gamma);
