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
public:
  struct Meta {
    GameState& state;
    Control& control;
    Camera& camera;
    float delta_time;
    float time;
  };

  using Handler = std::function<void (Meta& meta)>;

private:
  std::shared_ptr<Camera> _camera;
  std::vector<Handler> _handlers;
  glm::vec3 _cursor = zero<glm::vec3>();
  glm::vec3 _move_direction = zero<glm::vec3>();
  entt::registry _registry;
  entt::entity _player_id;
  ModelFactory _factory;
  unsigned long _last_time_point;

  template<typename T>
  void add_entities(std::vector<T>& source, std::vector<T>& destination) {
    std::copy(begin(source), end(source), std::back_inserter(destination));
  }

public:
  GameState();
  GameState(const GameState&) = delete;
  GameState& operator=(const GameState&) = delete;

  void camera(std::shared_ptr<Camera> camera);
  std::shared_ptr<Camera> camera();

  void cursor(glm::vec3 value);
  glm::vec3 cursor();

  void add_handler(Handler handler);
  void update(Control& control, unsigned long time_since_start_of_program);

  void player_id(entt::entity value);
  entt::entity player_id();

  entt::registry& registry();
  ModelFactory& factory();

  template<typename ...Components>
  decltype(auto) player() {
    return _registry.get<Components...>(_player_id);
  }
};
