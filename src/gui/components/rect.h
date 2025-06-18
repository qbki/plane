#pragma once
#include <entt/entt.hpp>
#include <memory>

import pln.components.common;
import pln.services.theme;
import pln.utils.color;

namespace GUI::Factory {

struct RectConfig
{
  float width { 0 };
  float height { 0 };
  pln::utils::color::Color color { pln::services::theme().components.text.color };
  pln::components::Parent parent { std::nullopt };
};

entt::entity
rect(std::shared_ptr<entt::registry>& registry, const RectConfig& config);

}
