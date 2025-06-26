module;
#include <entt/entt.hpp>
#include <memory>
#include <utility>

export module pln.scene.scene;

import pln.cameras.icamera;
import pln.collections.octree;
import pln.events.event_emitter;
import pln.game_state.state;
import pln.scene.iscene;
import pln.services.app;

using namespace pln::game_state;

namespace pln::scene {

export class Scene : public IScene
{
public:
  Scene(std::unique_ptr<pln::cameras::ICamera> camera)
    : IScene()
  {
    _state->camera(std::move(camera));
  }

  void
  update() override
  {
    if (!_is_paused) {
      const auto& control = pln::services::app().control();
      _events.emit(*this);
      auto should_emit_cancel = control.cancel && _is_allowed_to_cancel;
      if (should_emit_cancel) {
        _cancel_events.emit(*this);
      }
      _is_allowed_to_cancel = !control.cancel;
    }
  }

  pln::events::EventEmitter<IScene>&
  handlers() override
  {
    return _events;
  }

  pln::events::EventEmitter<IScene>&
  cancel_handlers() override
  {
    return _cancel_events;
  }

  State&
  state() const override
  {
    return *_state;
  }

  void
  is_deferred(bool value) override
  {
    _is_deferred = value;
  }

  bool
  is_deferred() const override
  {
    return _is_deferred;
  }

  void
  is_paused(bool value) override
  {
    _is_paused = value;
    if (_is_paused) {
      _is_allowed_to_cancel = false;
    }
  }

  bool
  is_paused() const override
  {
    return _is_paused;
  }

  void
  entities(std::unique_ptr<pln::collections::Octree<entt::entity>> value) override
  {
    _entities = std::move(value);
  }

  std::unique_ptr<pln::collections::Octree<entt::entity>>&
  entities() override
  {
    return _entities;
  }

private:
  bool _is_deferred = true;
  bool _is_paused = false;
  bool _is_allowed_to_cancel = false;

  std::unique_ptr<pln::collections::Octree<entt::entity>>
    _entities = std::make_unique<pln::collections::Octree<entt::entity>>();
  pln::events::EventEmitter<IScene> _events {};
  pln::events::EventEmitter<IScene> _cancel_events {};
  std::unique_ptr<State> _state = std::make_unique<State>();
};

}
