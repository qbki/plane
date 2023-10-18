#pragma once
#include <functional>
#include <glm/ext/vector_float3.hpp>
#include <memory>
#include <vector>

#include "../camera.h"
#include "../control.h"
#include "../models/index.h"
#include "enemy_state.h"
#include "projectile.h"

class GameState {
public:
  struct Meta {
    GameState& state;
    Control& control;
    Camera& camera;
    float seconds_since_last_frame;
  };

  using Entity = std::shared_ptr<Graphic>;
  using Entities = std::vector<Entity>;
  using Projectiles = std::vector<Projectile>;
  using EnemiesState = std::vector<EnemyState>;
  using Handler = std::function<void (Meta& meta)>;

private:
  std::shared_ptr<Camera> _camera;
  Entity _player;
  Projectiles _projectiles;
  EnemiesState _enemies_state;
  Entities _decorations;
  std::vector<Handler> _handlers;
  glm::vec3 _cursor = zero<glm::vec3>();
  glm::vec3 _move_direction = zero<glm::vec3>();

  template<typename T>
  void add_entities(std::vector<T>& source, std::vector<T>& destination) {
    std::copy(begin(source), end(source), std::back_inserter(destination));
  }

public:
  GameState() {}
  GameState(const GameState&) = delete;
  GameState& operator=(const GameState&) = delete;

  void camera(std::shared_ptr<Camera> camera);
  std::shared_ptr<Camera> camera();

  void player(Entity player);
  Entity player();

  void cursor(glm::vec3 value);
  glm::vec3 cursor();

  void add_projectiles(Projectiles& bullets);
  Projectiles& projectiles();

  void add_enemies(Entities& enemies);
  EnemiesState& enemies_state();

  void add_decoration(Entities& decorations);
  Entities& decorations();

  void add_handler(Handler handler);
  void update(Control& control, float seconds);
};
