module;
#include <entt/entt.hpp>
#include <string>

export module pln.gui.core.button_state;

import pln.components.common;
import pln.utils.crash;

using namespace pln::components;

namespace pln::gui::core {

export
class ButtonState
{
public:
  enum class State
  {
    ACTIVE,
    HOVER,
    DISABLED,
  };

private:
  entt::entity _active = entt::null;
  entt::entity _hover = entt::null;
  entt::entity _disabled = entt::null;
  bool _is_dirty = true;
  State _state = State::ACTIVE;

public:
  ButtonState(entt::entity active,
              entt::entity hover,
              entt::entity disabled)
    : _active(active)
    , _hover(hover)
    , _disabled(disabled)
  {
  }


  void
  apply(entt::registry& registry)
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
  state(State value)
  {
    _state = value;
  }


  [[nodiscard]] State
  state() const
  {
    return _state;
  }

  [[nodiscard]] bool
  is_active() const
  {
    return _state == State::ACTIVE;
  }

  [[nodiscard]] bool
  is_hovered() const
  {
    return _state == State::HOVER;
  }

  [[nodiscard]] bool
  is_disabled() const
  {
    return _state == State::DISABLED;
  }

  [[nodiscard]] entt::entity
  current() const
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

};

}
