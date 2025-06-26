module;
#include "src/gui/core/button_state.h"
#include "src/gui/core/theme.h"

export module pln.gui.components.text_button;

import pln.components.common;
import pln.components.transform;
import pln.events.event;
import pln.events.event_emitter;
import pln.gui.components.text;
import pln.services.theme;
import pln.shapes;

using namespace pln::components;
using namespace pln::events;

namespace pln::gui::components {

export
struct TextButtonConfig
{
  GUI::Typography::Font font { pln::services::theme().typography.h4 };
  pln::components::Parent parent { std::nullopt };
  std::string text {""};
  pln::events::EventEmitter<pln::events::PointerDown>::Handler on_pointer_down {
    [](auto&) {}
  };
  pln::events::EventEmitter<pln::events::PointerDown>::Handler on_pointer_down_once {
    [](auto&) {}
  };
};


export
entt::entity
text_button(std::shared_ptr<entt::registry>& registry,
            const TextButtonConfig& config)
{
  auto entity = registry->create();
  auto& theme = pln::services::theme();
  auto& button_theme = theme.components.text_button;

  auto active_entity = text(registry,
                            {
                              .color = button_theme.active.color,
                              .font = config.font,
                              .parent = Parent { entity },
                              .text = config.text,
                            });

  auto hover_entity = text(registry,
                           {
                             .color = button_theme.hover.color,
                             .font = config.font,
                             .parent = Parent { entity },
                             .text = config.text,
                           });

  auto disabled_entity = text(registry,
                              {
                                .color = button_theme.disabled.color,
                                .font = config.font,
                                .parent = Parent { entity },
                                .text = config.text,
                              });

  GUI::ButtonState button_state { active_entity, hover_entity, disabled_entity };
  button_state.apply(*registry);

  Children children { std::vector<entt::entity> { button_state.current() } };
  registry->emplace<GUI::ButtonState>(entity, button_state);
  registry->emplace<Children>(entity, children);
  registry->emplace<IsPointerDownEventAccepted>(entity, false);
  registry->emplace<IsPointerInside>(entity, false);
  registry->emplace<Parent>(entity, config.parent);
  registry->emplace<pln::shapes::RectSize>(entity, 0, 0);
  registry->emplace<Transform>(entity);

  registry->emplace<EventEmitter<PointerMove>>(entity);
  auto& pointer_down = registry
                         ->emplace<EventEmitter<PointerDown>>(
                           entity);
  auto& pointer_enter = registry
                          ->emplace<EventEmitter<PointerEnter>>(
                            entity);
  auto& pointer_leave = registry
                          ->emplace<EventEmitter<PointerLeave>>(
                            entity);
  auto& layout = registry->emplace<EventEmitter<GUILayout>>(
    entity);

  pointer_down.add(config.on_pointer_down);
  pointer_down.once(config.on_pointer_down_once);

  layout.add([registry, entity](auto&) {
    auto [state,
          parent_rect_size] = registry->get<GUI::ButtonState, pln::shapes::RectSize>(entity);
    const auto& child_rect_size = registry->get<pln::shapes::RectSize>(state.current());
    parent_rect_size = child_rect_size;
  });

  pointer_enter.add([registry, entity](auto&) {
    auto [state, children] = registry->get<GUI::ButtonState, Children>(entity);
    state.state(GUI::ButtonState::State::HOVER);
    state.apply(*registry);
  });

  pointer_leave.add([registry, entity](auto&) {
    auto [state, children] = registry->get<GUI::ButtonState, Children>(entity);
    state.state(GUI::ButtonState::State::ACTIVE);
    state.apply(*registry);
  });

  return entity;
}

}
