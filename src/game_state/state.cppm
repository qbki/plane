module;
#include <entt/entt.hpp> // IWYU pragma: export
#include <glm/gtc/constants.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <utility>

#include "src/math/shapes.h"    // IWYU pragma: export

export module pln.game_state.state;

import pln.cameras.icamera;

namespace pln::game_state {

export
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


  void
  camera(std::unique_ptr<pln::cameras::ICamera> camera)
  {
    _camera = std::move(camera);
  }


  [[nodiscard]] pln::cameras::ICamera&
  camera() const
  {
    return *_camera;
  }


  void
  cursor(glm::vec3 value)
  {
    _cursor = value;
  }


  [[nodiscard]] glm::vec3
  cursor() const
  {
    return _cursor;
  }


  void
  world_bbox(const Shape::AABB& value)
  {
    _world_bbox = value;
  }


  [[nodiscard]] const Shape::AABB&
  world_bbox() const
  {
    return _world_bbox;
  }


  entt::registry&
  registry()
  {
    return *_registry;
  }


  std::shared_ptr<entt::registry>&
  shared_registry()
  {
    return _registry;
  }


  void
  clear_registry()
  {
    _registry = std::make_shared<entt::registry>();
  }
};

}
