#pragma once
#include <entt/entt.hpp>

import pln.components.common;

namespace GUI::Factory {

struct BlockConfig
{
  pln::components::Children children { std::vector<entt::entity>() };
};

entt::entity
block(std::shared_ptr<entt::registry>& registry,
      const BlockConfig& config = {});

}
