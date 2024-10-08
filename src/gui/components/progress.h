#pragma once
#include <entt/entt.hpp>

#include "src/components/common.h"

namespace GUI::Factory {

struct ProgressConfig
{
  float width = 100.f; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
  float height = 10.f; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
  Parent parent = Parent { std::nullopt };
  int value = 0;
};

entt::entity
progress(std::shared_ptr<entt::registry>& registry,
         const ProgressConfig& config);

}
