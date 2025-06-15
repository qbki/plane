#pragma once
#include "src/components/common.h"

import pln.services.theme;
import pln.utils.color;

namespace GUI::Factory {

struct RectConfig
{
  float width { 0 };
  float height { 0 };
  pln::utils::color::Color color = pln::services::theme().components.text.color;
  Parent parent = Parent { std::nullopt };
};

entt::entity
rect(std::shared_ptr<entt::registry>& registry, const RectConfig& config);

}
