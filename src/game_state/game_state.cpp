#include <algorithm>
#include <glm/common.hpp>
#include <stdexcept>
#include <variant>

#include "src/utils/types.h"

#include "factory.h"
#include "game_state.h"

void
GameState::camera(std::shared_ptr<Camera> camera)
{
  _camera = camera;
}

std::shared_ptr<Camera>
GameState::camera()
{
  return _camera;
}

void
GameState::cursor(glm::vec3 value)
{
  _cursor = value;
}

glm::vec3
GameState::cursor() const
{
  return _cursor;
}

void
GameState::world_bbox(const Shape::AABB& value)
{
  _world_bbox = value;
}

void
GameState::extend_world_bbox(const Shape::Collider& value)
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
GameState::world_bbox() const
{
  return _world_bbox;
}

void
GameState::is_running(bool value)
{
  _is_game_running = value;
}

bool
GameState::is_running() const
{
  return _is_game_running;
}

void
GameState::player_id(entt::entity value)
{
  _player_id = value;
}

entt::entity
GameState::player_id() const
{
  return _player_id;
}

entt::registry&
GameState::registry()
{
  return _registry;
}

ModelFactory&
GameState::factory()
{
  return _factory;
}
