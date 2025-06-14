#pragma once
#include <string>

#include "src/components/common.h"
#include "src/services.h"

import pln.utils.color;

namespace GUI::Factory {

struct TextConfig
{
  pln::utils::color::Color color = Services::theme().components.text.color;
  GUI::Typography::Font font = Services::theme().typography.body1;
  Parent parent = Parent { std::nullopt };
  std::string text = "";
};

entt::entity
text(std::shared_ptr<entt::registry>& registry, const TextConfig& config);

}
