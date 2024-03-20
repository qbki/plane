#include "game_state.h"
#include "factory.h"

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
