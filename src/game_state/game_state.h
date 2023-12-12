#pragma once
#include <entt/entt.hpp>
#include <functional>
#include <memory>
#include <vector>

#include "../camera.h"
#include "../components.h"
#include "../control.h"
#include "../models/index.h"
#include "factory.h"


class GameState {
private:
  std::shared_ptr<Camera> _camera;
  glm::vec3 _cursor = zero<glm::vec3>();
  glm::vec3 _move_direction = zero<glm::vec3>();
  entt::registry _registry;
  entt::entity _player_id;
  ModelFactory _factory;
  bool _is_game_running = true;

  template<typename T>
  void add_entities(std::vector<T>& source, std::vector<T>& destination) {
    std::copy(begin(source), end(source), std::back_inserter(destination));
  }

public:
  GameState() {};
  GameState(const GameState&) = delete;
  GameState& operator=(const GameState&) = delete;

  void camera(std::shared_ptr<Camera> camera);
  std::shared_ptr<Camera> camera();

  void cursor(glm::vec3 value);
  glm::vec3 cursor();

  void is_running(bool value);
  bool is_running();

  void player_id(entt::entity value);
  entt::entity player_id();

  entt::registry& registry();
  ModelFactory& factory();

  template<typename ...Components>
  decltype(auto) player() {
    return _registry.get<Components...>(_player_id);
  }
};
