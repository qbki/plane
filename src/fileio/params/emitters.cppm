module;
#include <entt/entt.hpp>
#include <glm/common.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <string>

export module pln.fileio.params.emitters;

import pln.components.common;
import pln.components.transform;
import pln.fileio.params.entities;
import pln.game_state.factory;
import pln.game_state.state;

using namespace pln::components;
using namespace pln::fileio::params;
using namespace pln::game_state::factory;

namespace pln::fileio::params {

glm::quat
calc_rotation(unsigned int multiplier, float angle)
{
  return glm::angleAxis(static_cast<float>(multiplier) * angle,
                        glm::vec3(0, 0, 1));
}


export
void
emit_particles(std::shared_ptr<entt::registry> registry,
               glm::vec3 initial_position,
               const EntityParamsParticles& params,
               const std::string& file_path)
{
  unsigned int idx = 0;
  const float step = glm::two_pi<float>() / static_cast<float>(params.quantity);
  auto particles = registry->view<Transform,
                                  Lifetime,
                                  LifetimeMax,
                                  Speed,
                                  Velocity,
                                  ParticleKind>(entt::exclude<Available>);

  for (auto [id, transform, lifetime, lifetime_max, speed, velocity] :
       particles.each()) {
    auto rotation = calc_rotation(idx, step);
    transform.translate(initial_position).rotate(rotation);
    velocity.value = rotation * glm::vec3(1, 0, 0) * speed.value;
    registry->emplace<Available>(id);
    lifetime.value = lifetime_max.value;
    idx += 1;
    if (idx >= (params.quantity - 1)) {
      break;
    }
  }

  for (; idx < params.quantity; idx += 1) {
    auto entity = make_particle(registry, file_path);
    auto rotation = calc_rotation(idx, step);
    Transform transform;
    transform.translate(initial_position).rotate(rotation);
    Velocity velocity { rotation * glm::vec3(1, 0, 0) * params.speed };
    registry->replace<Lifetime>(entity, params.lifetime);
    registry->replace<LifetimeMax>(entity, params.lifetime);
    registry->replace<Transform>(entity, transform);
    registry->replace<Velocity>(entity, velocity);
  }
}

}
