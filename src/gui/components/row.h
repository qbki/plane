#pragma once
#include <entt/entt.hpp>

#include "src/components/common.h"

namespace GUI::Factory {

struct RowConfig
{
  Children children = Children { std::vector<entt::entity>() };
};

entt::entity
row(std::shared_ptr<entt::registry>& registry, const RowConfig& config = {});

}
