#include <utility>

#include "src/game_state/state.h"

#include "scene.h"

Scene::Scene(std::unique_ptr<Camera> camera)
{
  _state->camera(std::move(camera));
}

void
Scene::update()
{
  _events.emit(*this);
}

Events::EventEmitter<Scene>&
Scene::handlers()
{
  return _events;
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
