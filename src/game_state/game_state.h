#pragma once
#include <entt/entt.hpp>
#include <memory>

#include "../camera.h"
#include "../utils/index.h"
#include "factory.h"

class GameState
{
private:
  std::shared_ptr<Camera> _camera;
  glm::vec3 _cursor = zero<glm::vec3>();
  entt::registry _registry;
  entt::entity _player_id = entt::null;
  ModelFactory _factory;
  bool _is_game_running = true;

public:
  GameState() = default;
  GameState(const GameState&) = delete;
  GameState& operator=(const GameState&) = delete;
  GameState(GameState&&) = delete;
  GameState& operator=(GameState&&) = delete;
  ~GameState() = default;

  void camera(std::shared_ptr<Camera> camera);
  std::shared_ptr<Camera> camera();

  void cursor(glm::vec3 value);
  [[nodiscard]] glm::vec3 cursor() const;

  void is_running(bool value);
  [[nodiscard]] bool is_running() const;

  void player_id(entt::entity value);
  [[nodiscard]] entt::entity player_id() const;

  entt::registry& registry();
  ModelFactory& factory();

  template<typename... Components>
  decltype(auto) player()
  {
    return _registry.get<Components...>(_player_id);
  }
};
