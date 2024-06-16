#include <algorithm>
#include <glm/common.hpp>
#include <memory>
#include <stdexcept>
#include <utility>
#include <variant>

#include "src/utils/types.h"

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

void
State::extend_world_bbox(const Shape::Collider& value)
{
  std::visit(
    Overloaded{
      [&](const Shape::Sphere& collider) {
        auto min_vector = collider.center - glm::vec3(collider.radius);
        auto max_vector = collider.center + glm::vec3(collider.radius);
        auto max = std::max({ max_vector.x,
                              max_vector.y,
                              max_vector.z,
                              glm::abs(min_vector.x),
                              glm::abs(min_vector.y),
                              glm::abs(min_vector.z),
                              _world_bbox.max.x });
        _world_bbox.min = glm::vec3(-max);
        _world_bbox.max = glm::vec3(max);
      },
      [](const auto&) {
        throw std::runtime_error(
          "Found an unsupported collider while extending the world bbox");
      },
    },
    value);
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
