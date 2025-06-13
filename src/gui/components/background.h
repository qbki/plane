#pragma once
#include "src/components/common.h"
#include "src/services.h"

import utils.color;

namespace GUI::Factory {

struct BackgroundConfig
{
  utils::color::Color color = { Services::theme().palette.white };
  Parent parent = Parent { std::nullopt };
  float z { 0 };
};

entt::entity
background(std::shared_ptr<entt::registry>& registry,
           const BackgroundConfig& config = {});

}
