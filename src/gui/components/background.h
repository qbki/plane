#pragma once
#include "src/components/common.h"
#include "src/services.h"
#include "src/utils/color.h"

namespace GUI::Factory {

struct BackgroundConfig
{
  Core::Color color = { Services::theme().palette.white };
  Parent parent = Parent { std::nullopt };
  float z = 0;
};

entt::entity
background(std::shared_ptr<entt::registry>& registry,
           const BackgroundConfig& config = {});

}
