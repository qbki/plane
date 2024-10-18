#pragma once
#include <memory>

#include "src/cameras/camera.h"
#include "src/events/event_emitter.h" // IWYU pragma: export
#include "src/game_state/state.h"     // IWYU pragma: export

class Scene
{
private:
  bool _is_deferred = true;
  bool _is_paused = false;

  bool _is_allowed_to_cancel = false;

  Events::EventEmitter<Scene> _events {};
  Events::EventEmitter<Scene> _cancel_events {};
  std::unique_ptr<State> _state = std::make_unique<State>();

public:
  explicit Scene(std::unique_ptr<Camera> camera);

  void update();
  Events::EventEmitter<Scene>& handlers();
  Events::EventEmitter<Scene>& cancel_handlers();
  [[nodiscard]] State& state() const;

  void is_deferred(bool value);
  [[nodiscard]] bool is_deferred() const;

  void is_paused(bool value);
  [[nodiscard]] bool is_paused() const;
};
