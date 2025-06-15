#pragma once
#include <string>

#include "src/components/common.h"
#include "src/gui/core/theme.h"

import pln.utils.color;
import pln.services.theme;

namespace GUI::Factory {

struct TextConfig
{
  pln::utils::color::Color color = pln::services::theme().components.text.color;
  GUI::Typography::Font font = pln::services::theme().typography.body1;
  Parent parent = Parent { std::nullopt };
  std::string text = "";
};

entt::entity
text(std::shared_ptr<entt::registry>& registry, const TextConfig& config);

}
