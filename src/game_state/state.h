#pragma once
#include <entt/entt.hpp> // IWYU pragma: export
#include <glm/gtc/constants.hpp>
#include <glm/vec3.hpp>
#include <memory> // IWYU pragma: export

#include "src/math/shapes.h"    // IWYU pragma: export

import pln.cameras.icamera;

class State
{
private:
  std::unique_ptr<pln::cameras::ICamera> _camera;
  glm::vec3 _cursor = glm::zero<glm::vec3>();
  std::shared_ptr<entt::registry>
    _registry = std::make_shared<entt::registry>();
  Shape::AABB _world_bbox;

public:
  State() = default;
  State(const State&) = delete;
  State& operator=(const State&) = delete;
  State(State&&) = delete;
  State& operator=(State&&) = delete;
  ~State() = default;

  void camera(std::unique_ptr<pln::cameras::ICamera> camera);
  [[nodiscard]] pln::cameras::ICamera& camera() const;

  void cursor(glm::vec3 value);
  [[nodiscard]] glm::vec3 cursor() const;

  void world_bbox(const Shape::AABB& value);
  [[nodiscard]] const Shape::AABB& world_bbox() const;

  entt::registry& registry();
  std::shared_ptr<entt::registry>& shared_registry();
  void clear_registry();
};
