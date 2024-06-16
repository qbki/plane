#pragma once
#include <entt/entt.hpp>
#include <glm/mat4x4.hpp>

#include "src/components/common.h"

glm::mat4
get_global_matrix(entt::registry& registry, const Parent& parent);
