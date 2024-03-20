#pragma once
#include <entt/entt.hpp> // IWYU pragma: export
#include <glm/gtc/constants.hpp>
#include <glm/vec3.hpp>
#include <memory> // IWYU pragma: export

#include "src/camera.h" // IWYU pragma: export

#include "factory.h"

class GameState
{
private:
  std::shared_ptr<Camera> _camera;
  glm::vec3 _cursor = glm::zero<glm::vec3>();
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
