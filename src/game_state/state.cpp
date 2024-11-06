#include <memory>
#include <utility>

#include "state.h"

void
State::camera(std::unique_ptr<Camera> camera)
{
  _camera = std::move(camera);
}

Camera&
State::camera() const
{
  return *_camera;
}

void
State::cursor(glm::vec3 value)
{
  _cursor = value;
}

glm::vec3
State::cursor() const
{
  return _cursor;
}

void
State::world_bbox(const Shape::AABB& value)
{
  _world_bbox = value;
}

const Shape::AABB&
State::world_bbox() const
{
  return _world_bbox;
}

entt::registry&
State::registry()
{
  return *_registry;
}

std::shared_ptr<entt::registry>&
State::shared_registry()
{
  return _registry;
}

void
State::clear_registry()
{
  _registry = std::make_shared<entt::registry>();
}
