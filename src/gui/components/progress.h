#pragma once
#include <entt/entt.hpp>

import pln.components.common;

namespace GUI::Factory {

struct ProgressConfig
{
  float width { 100.f }; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
  float height { 10.f }; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
  pln::components::Parent parent { std::nullopt };
  int value { 0 };
};

entt::entity
progress(std::shared_ptr<entt::registry>& registry,
         const ProgressConfig& config);

}
