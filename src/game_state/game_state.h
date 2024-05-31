#pragma once
#include <entt/entt.hpp> // IWYU pragma: export
#include <glm/gtc/constants.hpp>
#include <glm/vec3.hpp>
#include <memory> // IWYU pragma: export

#include "src/cameras/camera.h" // IWYU pragma: export

#include "src/math/shapes.h"

class GameState
{
private:
  std::unique_ptr<Camera> _camera;
  std::unique_ptr<Camera> _gui_camera;
  glm::vec3 _cursor = glm::zero<glm::vec3>();
  entt::registry _registry;
  entt::entity _player_id = entt::null;
  bool _is_game_running = true;
  Shape::AABB _world_bbox;

public:
  GameState() = default;
  GameState(const GameState&) = delete;
  GameState& operator=(const GameState&) = delete;
  GameState(GameState&&) = delete;
  GameState& operator=(GameState&&) = delete;
  ~GameState() = default;

  void camera(std::unique_ptr<Camera>& camera);
  [[nodiscard]] Camera& camera() const;

  void gui_camera(std::unique_ptr<Camera>& camera);
  [[nodiscard]] Camera& gui_camera() const;

  void cursor(glm::vec3 value);
  [[nodiscard]] glm::vec3 cursor() const;

  void world_bbox(const Shape::AABB& value);
  void extend_world_bbox(const Shape::Collider& value);
  [[nodiscard]] const Shape::AABB& world_bbox() const;

  void is_running(bool value);
  [[nodiscard]] bool is_running() const;

  void player_id(entt::entity value);
  [[nodiscard]] entt::entity player_id() const;

  entt::registry& registry();

  template<typename... Components>
  decltype(auto) player()
  {
    return _registry.get<Components...>(_player_id);
  }
};
