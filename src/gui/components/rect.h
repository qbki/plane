#pragma once
#include "src/components/common.h"
#include "src/services.h"

import utils.color;

namespace GUI::Factory {

struct RectConfig
{
  float width = 0;
  float height = 0;
  utils::color::Color color = Services::theme().components.text.color;
  Parent parent = Parent { std::nullopt };
};

entt::entity
rect(std::shared_ptr<entt::registry>& registry, const RectConfig& config);

}
