#include "src/components/transform.h"
#include "src/gui/core/button_state.h"

#include "text.h"
#include "text_button.h"

import pln.components.common;
import pln.services.theme;
import pln.shapes;

using namespace pln::components;

namespace GUI::Factory {

entt::entity
text_button(std::shared_ptr<entt::registry>& registry,
            const TextButtonConfig& config)
{
  auto entity = registry->create();
  auto& theme = pln::services::theme();
  auto& button_theme = theme.components.text_button;

  auto active_entity = Factory::text(registry,
                                     {
                                       .color = button_theme.active.color,
                                       .font = config.font,
                                       .parent = Parent { entity },
                                       .text = config.text,
                                     });

  auto hover_entity = Factory::text(registry,
                                    {
                                      .color = button_theme.hover.color,
                                      .font = config.font,
                                      .parent = Parent { entity },
                                      .text = config.text,
                                    });

  auto disabled_entity = Factory::text(registry,
                                       {
                                         .color = button_theme.disabled.color,
                                         .font = config.font,
                                         .parent = Parent { entity },
                                         .text = config.text,
                                       });

  ButtonState button_state { active_entity, hover_entity, disabled_entity };
  button_state.apply(*registry);

  Children children { std::vector<entt::entity> { button_state.current() } };
  registry->emplace<ButtonState>(entity, button_state);
  registry->emplace<Children>(entity, children);
  registry->emplace<IsPointerDownEventAccepted>(entity, false);
  registry->emplace<IsPointerInside>(entity, false);
  registry->emplace<Parent>(entity, config.parent);
  registry->emplace<pln::shapes::RectSize>(entity, 0, 0);
  registry->emplace<Transform>(entity);

  registry->emplace<Events::EventEmitter<Events::PointerMove>>(entity);
  auto& pointer_down = registry
                         ->emplace<Events::EventEmitter<Events::PointerDown>>(
                           entity);
  auto& pointer_enter = registry
                          ->emplace<Events::EventEmitter<Events::PointerEnter>>(
                            entity);
  auto& pointer_leave = registry
                          ->emplace<Events::EventEmitter<Events::PointerLeave>>(
                            entity);
  auto& layout = registry->emplace<Events::EventEmitter<Events::GUILayout>>(
    entity);

  pointer_down.add(config.on_pointer_down);
  pointer_down.once(config.on_pointer_down_once);

  layout.add([registry, entity](auto&) {
    auto [state,
          parent_rect_size] = registry->get<ButtonState, pln::shapes::RectSize>(entity);
    const auto& child_rect_size = registry->get<pln::shapes::RectSize>(state.current());
    parent_rect_size = child_rect_size;
  });

  pointer_enter.add([registry, entity](auto&) {
    auto [state, children] = registry->get<ButtonState, Children>(entity);
    state.state(ButtonState::State::HOVER);
    state.apply(*registry);
  });

  pointer_leave.add([registry, entity](auto&) {
    auto [state, children] = registry->get<ButtonState, Children>(entity);
    state.state(ButtonState::State::ACTIVE);
    state.apply(*registry);
  });

  return entity;
}

}
