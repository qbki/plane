#pragma once
#include <memory>

#include "src/cameras/camera.h"
#include "src/events/event_emitter.h" // IWYU pragma: export
#include "src/game_state/state.h"     // IWYU pragma: export

class Scene
{
private:
  Events::EventEmitter<Scene> _events{};
  std::unique_ptr<State> _state = std::make_unique<State>();

public:
  explicit Scene(std::unique_ptr<Camera> camera);

  void update();
  Events::EventEmitter<Scene>& handlers();
  State& state() const;
};
