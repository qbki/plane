#pragma once
#include <entt/entt.hpp>

#include "src/components/common.h"
#include "src/gui/core/theme.h"
#include "src/services.h"
#include "src/utils/color.h"

namespace GUI::Factory {

struct RectConfig
{
  float width = 0;
  float height = 0;
  Core::Color color = Services::theme().components.text.color;
  Parent parent = Parent { std::nullopt };
};

entt::entity
rect(std::shared_ptr<entt::registry>& registry, const RectConfig& config);

}
