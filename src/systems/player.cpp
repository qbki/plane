#define GLM_ENABLE_EXPERIMENTAL
#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/trigonometric.hpp>
#include <string>

#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/components/weapon.h"
#include "src/game_state/factory.h"
#include "src/scene/scene.h"
#include "src/services.h"
#include "src/utils/common.h"

#include "player.h"

void
player_rotation_system(const Scene& scene)
{
  static const auto tilt_angle = glm::pi<float>() / 4.0f;
  static const auto max_speed_coeficient = 0.4f;
  scene.state()
    .registry()
    .view<Transform, Velocity, AccelerationScalar, PlayerKind>()
    .each([&](Transform& transform,
              const Velocity& velocity,
              const AccelerationScalar& accel) {
      float x_tilt = 0;
      float y_tilt = 0;
      auto max_speed = accel.value * max_speed_coeficient;
      if (max_speed > 0) {
        x_tilt = -velocity.value.y / max_speed;
        y_tilt = velocity.value.x / max_speed;
      }

      auto direction = scene.state().cursor() - transform.translation();
      auto x = glm::angleAxis(tilt_angle * x_tilt, glm::vec3(1, 0, 0));
      auto y = glm::angleAxis(tilt_angle * y_tilt, glm::vec3(0, 1, 0));
      auto z = glm::angleAxis(glm::atan(direction.y, direction.x),
                              glm::vec3(0, 0, 1));
      transform.rotate(x * y * z);
    });
}

void
player_shooting_system(const Scene& scene)
{
  const auto& control = Services::app().control();
  scene.state().registry().view<Weapon, PlayerKind>().each(
    [&control](Weapon& weapon) { weapon.is_shooting = control.shooting; });
};

void
player_moving_system(const Scene& scene)
{
  const auto& control = Services::app().control();
  glm::vec3 move_direction { 0, 0, 0 };
  if (control.left) {
    move_direction.x -= 1;
  }
  if (control.right) {
    move_direction.x += 1;
  }
  if (control.up) {
    move_direction.y += 1;
  }
  if (control.down) {
    move_direction.y -= 1;
  }
  auto direction = is_approx_equal(glm::length2(move_direction), 0.0f)
                     ? move_direction
                     : glm::normalize(move_direction);
  scene.state()
    .registry()
    .view<Acceleration, AccelerationScalar, PlayerKind>()
    .each([&direction](Acceleration& accel,
                       const AccelerationScalar& accel_scalar) {
      accel.value += direction * accel_scalar.value;
    });
}

void
player_enemy_pointers(const Scene& scene)
{
  auto registry = scene.state().shared_registry();
  glm::vec3 player_position { 0, 0, 0 };
  std::vector<glm::vec3> enemy_positions;
  std::vector<entt::entity> destroy_list;

  registry->view<Transform, PlayerKind>().each([&](const Transform& transform) {
    player_position = transform.translation();
  });
  registry->view<Transform, EnemyKind, Available>().each(
    [&](const Transform& transform) {
      enemy_positions.push_back(transform.translation());
    });

  auto it_start = enemy_positions.begin();
  auto it_end = enemy_positions.end();
  auto is_finished = [&]() { return it_start == it_end; };
  auto is_far = [](const glm::vec3& a, const glm::vec3& b) {
    static const float distance = 15.0;
    return glm::distance(a, b) > distance;
  };
  auto calc_position = [](const glm::vec3& orig, const glm::vec3& enemy) {
    static const float distance = 2.0;
    return orig + glm::normalize(enemy - orig) * distance;
  };

  registry->view<Transform, EnemyPointer>().each(
    [&](entt::entity entity, Transform& transform) {
      if (is_finished()) {
        destroy_list.push_back(entity);
      } else if (!is_far(player_position, *it_start)) {
        destroy_list.push_back(entity);
        it_start++;
      } else {
        auto pos = calc_position(player_position, *it_start);
        transform.translate(pos);
        it_start++;
      }
    });
  for (auto entity : destroy_list) {
    registry->destroy(entity);
  }
  while (!is_finished()) {
    if (is_far(player_position, *it_start)) {
      auto entity = ModelFactory::make_rect(registry, "");
      auto& transform = registry->get<Transform>(entity);
      auto pos = calc_position(player_position, *it_start);
      transform.translate(pos);
    }
    it_start++;
  }
}

void
LoseSystem::operator()(const Scene& scene)
{
  if (is_fired) {
    return;
  }

  auto players_quantity = scene.state().registry().view<PlayerKind>().size();

  if (players_quantity == 0) {
    Services::app().pause_scenes();
    Services::app().add_once_handler(
      [](auto&) { Services::events<const Events::LoseEvent>().emit({}); });
    is_fired = true;
  }
}

void
player_updating_app_info_system(const Scene& scene)
{
  auto registry = scene.state().shared_registry();
  registry->view<Transform, PlayerKind>().each([&](const Transform& transform) {
    Services::app().info().player_position = transform.translation();
  });
}
