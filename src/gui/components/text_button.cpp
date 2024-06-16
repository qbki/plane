#include <entt/entt.hpp>

#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/gui/core/button_state.h"
#include "src/gui/core/theme.h"
#include "src/gui/types.h"
#include "src/services.h"

#include "text.h"
#include "text_button.h"

namespace GUI::Factory {

entt::entity
make_text_button(std::shared_ptr<entt::registry>& registry,
                 const std::string& text)
{
  auto entity = registry->create();
  auto& theme = Services::theme();
  auto& button_theme = theme.components.text_button;

  auto active_entity = make_text(*registry, text);
  registry->replace<Core::Color>(active_entity, button_theme.active.color);
  registry->replace<FontPtr>(active_entity, theme.typography.h4);
  registry->replace<Parent>(active_entity, entity);

  auto hover_entity = make_text(*registry, text);
  registry->replace<Core::Color>(hover_entity, button_theme.hover.color);
  registry->replace<FontPtr>(hover_entity, theme.typography.h4);
  registry->replace<Parent>(hover_entity, entity);

  auto disabled_entity = make_text(*registry, text);
  registry->replace<Core::Color>(disabled_entity, button_theme.disabled.color);
  registry->replace<FontPtr>(disabled_entity, theme.typography.h4);
  registry->replace<Parent>(disabled_entity, entity);

  ButtonState button_state{ active_entity, hover_entity, disabled_entity };
  button_state.apply(*registry);

  Children children{ std::vector<entt::entity>{ button_state.current() } };
  registry->emplace<ButtonState>(entity, button_state);
  registry->emplace<Children>(entity, children);
  registry->emplace<Events::EventEmitter<Events::PointerDown>>(entity);
  registry->emplace<IsPointerInside>(entity, false);
  registry->emplace<Parent>(entity, std::nullopt);
  registry->emplace<RectSize>(entity, 0, 0);
  registry->emplace<Transform>(entity);

  registry->emplace<Events::EventEmitter<Events::PointerMove>>(entity);
  auto& pointer_enter =
    registry->emplace<Events::EventEmitter<Events::PointerEnter>>(entity);
  auto& pointer_leave =
    registry->emplace<Events::EventEmitter<Events::PointerLeave>>(entity);
  auto& layout =
    registry->emplace<Events::EventEmitter<Events::GUILayout>>(entity);

  layout.add([registry, entity](auto&) {
    auto [state, parent_rect_size] =
      registry->get<ButtonState, RectSize>(entity);
    const auto& child_rect_size = registry->get<RectSize>(state.current());
    parent_rect_size = child_rect_size;
  });

  pointer_enter.add([registry, entity](Events::PointerEnter&) {
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
