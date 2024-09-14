#pragma once
#include <entt/entt.hpp>
#include <string>

#include "src/components/common.h"
#include "src/gui/core/theme.h"
#include "src/services.h"

#include "../types.h"

namespace GUI::Factory {

struct TextButtonConfig
{
  FontPtr font = Services::theme().typography.h4;
  Parent parent = Parent{ std::nullopt };
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
