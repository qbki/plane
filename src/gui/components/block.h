#pragma once
#include <entt/entt.hpp>

#include "src/components/common.h"

namespace GUI::Factory {

struct BlockConfig
{
  Children children = Children { std::vector<entt::entity>() };
};

entt::entity
block(std::shared_ptr<entt::registry>& registry,
      const BlockConfig& config = {});

}
