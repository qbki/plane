#pragma once
#include <string>

#include "src/components/common.h"
#include "src/services.h"

namespace GUI::Factory {

struct TextButtonConfig
{
  Typography::Font font = Services::theme().typography.h4;
  Parent parent = Parent { std::nullopt };
  std::string text = "";
  Events::EventEmitter<Events::PointerDown>::Handler on_pointer_down =
    [](auto&) {};
  Events::EventEmitter<Events::PointerDown>::Handler on_pointer_down_once =
    [](auto&) {};
};

entt::entity
text_button(std::shared_ptr<entt::registry>& registry,
            const TextButtonConfig& config);

}
