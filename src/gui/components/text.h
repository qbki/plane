#pragma once
#include <string>

#include "src/components/common.h"
#include "src/services.h"
#include "src/utils/color.h"

namespace GUI::Factory {

struct TextConfig
{
  Core::Color color = Services::theme().components.text.color;
  GUI::Typography::Font font = Services::theme().typography.body1;
  Parent parent = Parent { std::nullopt };
  std::string text = "";
};

entt::entity
text(std::shared_ptr<entt::registry>& registry, const TextConfig& config);

}
