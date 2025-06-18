#pragma once
#include <entt/entt.hpp>
#include <memory>
#include <optional>

import pln.components.common;
import pln.services.theme;
import pln.utils.color;

namespace GUI::Factory {

struct BackgroundConfig
{
  pln::utils::color::Color color { pln::services::theme().palette.white };
  pln::components::Parent parent { std::nullopt };
  float z { 0 };
};

entt::entity
background(std::shared_ptr<entt::registry>& registry,
           const BackgroundConfig& config = {});

}
