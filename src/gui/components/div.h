#pragma once
#include <entt/entt.hpp>

#include "src/components/common.h"

namespace GUI::Factory {

struct DivConfig
{
  float width = 0;
  float height = 0;
  Parent parent = Parent{ std::nullopt };
  Children children = Children{ std::vector<entt::entity>() };
};

entt::entity
div(std::shared_ptr<entt::registry>& registry, const DivConfig& config);

}
