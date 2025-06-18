#pragma once
#include <entt/entt.hpp>

import pln.components.common;

namespace GUI::Factory {

struct RowConfig
{
  pln::components::Children children { std::vector<entt::entity>() };
};

entt::entity
row(std::shared_ptr<entt::registry>& registry, const RowConfig& config = {});

}
