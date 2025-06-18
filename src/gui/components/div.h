#pragma once
#include <entt/entt.hpp>

import pln.components.common;

namespace GUI::Factory {

struct DivConfig
{
  float width { 0 };
  float height { 0 };
  pln::components::Parent parent { std::nullopt };
  pln::components::Children children { std::vector<entt::entity>() };
};

entt::entity
div(std::shared_ptr<entt::registry>& registry, const DivConfig& config);

}
