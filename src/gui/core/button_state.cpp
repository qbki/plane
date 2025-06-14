#include <entt/entt.hpp>
#include <string>

#include "src/components/common.h"

#include "button_state.h"

import utils.crash;


namespace GUI {

ButtonState::ButtonState(entt::entity active,
                         entt::entity hover,
                         entt::entity disabled)
  : _active(active)
  , _hover(hover)
  , _disabled(disabled)
{
}

void
ButtonState::apply(entt::registry& registry)
{
  if (!_is_dirty) {
    return;
  }
  auto add = [&registry](entt::entity entity) {
    registry.emplace_or_replace<Available>(entity);
  };
  auto remove = [&registry](entt::entity entity) {
    registry.remove<Available>(entity);
  };
  remove(_active);
  remove(_disabled);
  remove(_hover);
  switch (_state) {
    case State::ACTIVE:
      add(_active);
      break;
    case State::DISABLED:
      add(_disabled);
      break;
    case State::HOVER:
      add(_hover);
      break;
  }
}

void
ButtonState::state(State value)
{
  _state = value;
}

ButtonState::State
ButtonState::state() const
{
  return _state;
}

bool
ButtonState::is_active() const
{
  return _state == State::ACTIVE;
}

bool
ButtonState::is_hovered() const
{
  return _state == State::HOVER;
}

bool
ButtonState::is_disabled() const
{
  return _state == State::DISABLED;
}

entt::entity
ButtonState::current() const
{
  switch (_state) {
    case State::ACTIVE:
      return _active;
    case State::DISABLED:
      return _disabled;
    case State::HOVER:
      return _hover;
  }
  pln::utils::crash("Encountered an unknown button state");
}

}
