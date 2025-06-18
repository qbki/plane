#pragma once
#include <string>

#include "src/events/event_emitter.h"
#include "src/gui/core/theme.h"

import pln.components.common;
import pln.services.theme;

namespace GUI::Factory {

struct TextButtonConfig
{
  Typography::Font font { pln::services::theme().typography.h4 };
  pln::components::Parent parent { std::nullopt };
  std::string text = "";
  Events::EventEmitter<Events::PointerDown>::Handler on_pointer_down {
    [](auto&) {}
  };
  Events::EventEmitter<Events::PointerDown>::Handler on_pointer_down_once {
    [](auto&) {}
  };
};

entt::entity
text_button(std::shared_ptr<entt::registry>& registry,
            const TextButtonConfig& config);

}
