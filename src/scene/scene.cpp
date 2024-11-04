#include <utility>

#include "src/game_state/state.h"
#include "src/scene/scene.h"
#include "src/services.h"

#include "scene.h"

Scene::Scene(std::unique_ptr<Camera> camera)
{
  _state->camera(std::move(camera));
}

void
Scene::update()
{
  if (!_is_paused) {
    const auto& control = Services::app().control();
    _events.emit(*this);
    auto should_emit_cancel = control.cancel && _is_allowed_to_cancel;
    if (should_emit_cancel) {
      _cancel_events.emit(*this);
    }
    _is_allowed_to_cancel = !control.cancel;
  }
}

Events::EventEmitter<Scene>&
Scene::handlers()
{
  return _events;
}

Events::EventEmitter<Scene>&
Scene::cancel_handlers()
{
  return _cancel_events;
}

State&
Scene::state() const
{
  return *_state;
}

void
Scene::is_deferred(bool value)
{
  _is_deferred = value;
}

bool
Scene::is_deferred() const
{
  return _is_deferred;
}

void
Scene::is_paused(bool value)
{
  _is_paused = value;
  if (_is_paused) {
    _is_allowed_to_cancel = false;
  }
}

bool
Scene::is_paused() const
{
  return _is_paused;
}

void
Scene::entities(std::unique_ptr<Octree<entt::entity>> value)
{
  _entities = std::move(value);
}

std::unique_ptr<Octree<entt::entity>>&
Scene::entities()
{
  return _entities;
}
