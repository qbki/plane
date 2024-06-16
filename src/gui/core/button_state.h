#pragma once
#include <entt/entt.hpp>

namespace GUI {

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
  ButtonState(entt::entity active, entt::entity hover, entt::entity disabled);

  void apply(entt::registry& registry);

  void state(State value);
  [[nodiscard]] State state() const;

  [[nodiscard]] bool is_active() const;
  [[nodiscard]] bool is_hovered() const;
  [[nodiscard]] bool is_disabled() const;

  [[nodiscard]] entt::entity current() const;
};

}
