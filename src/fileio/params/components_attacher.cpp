#include <variant>

#include "src/components/common.h"
#include "src/components/linear_velocity.h"
#include "src/components/transform.h"
#include "src/components/velocity.h"
#include "src/components/weapon.h"
#include "src/game_state/factory.h"
#include "src/utils/noop.h"

#include "components_attacher.h"
#include "emitters.h"
#include "entities_map.h"

ComponetsAttacher::ComponetsAttacher(Scene* scene,
                                     const entt::entity entity,
                                     const EntityParamsMap* params_map)
  : _scene(scene)
  , _entity(entity)
  , _registry(&scene->state().registry())
  , _entities(params_map)
{
}

void
ComponetsAttacher::operator()(const EntityParamsActor& params) const
{
  attach_linear_velocity(params.speed);
  attach_particles_emmiter_by_hit(params);
  attach_debris_emmiter(params);
  attach_lives(params.lives);
  attach_weapon(params);
  auto model = _entities->model(params.model_id);
  (*this)(model);
}

void
ComponetsAttacher::operator()(const EntityParamsDirectionalLight& params) const
{
  attach_color(params.color);
  attach_direction(params.direction);
}

void
ComponetsAttacher::operator()(const EntityParamsPointLight& params) const
{
  attach_color(params.color);
}

void
ComponetsAttacher::operator()(const EntityParamsWeapon&) const
{
  noop();
}

void
ComponetsAttacher::operator()(const EntityParamsModel& params) const
{
  attach_opaque(params.is_opaque);
}

void
ComponetsAttacher::operator()(const EntityParamsParticles&) const
{
}

void
ComponetsAttacher::operator()(const EntityParamsTutorialButton& params) const
{
  attach_tutorial_button_value(params.button);
  auto model = _entities->model(params.model_id);
  (*this)(model);
}

void
ComponetsAttacher::attach(const EntityParams& params) const
{
  std::visit(*this, params);
}

void
ComponetsAttacher::attach_linear_velocity(const float& speed) const
{
  _registry->emplace_or_replace<LinearVelocity>(_entity, speed);
}

void
ComponetsAttacher::attach_lives(int lives) const
{
  _registry->emplace_or_replace<Lives>(_entity, lives);
}

void
ComponetsAttacher::attach_velocity(const VelocityParams& velocity) const
{
  _registry->emplace_or_replace<Velocity>(
    _entity, velocity.acceleration, velocity.damping);
}

void
ComponetsAttacher::attach_opaque(bool is_opaque) const
{
  if (is_opaque) {
    _registry->emplace_or_replace<Opaque>(_entity);
  }
}

void
ComponetsAttacher::attach_direction(const glm::vec3& direction) const
{
  _registry->emplace_or_replace<Direction>(_entity, direction);
}

void
ComponetsAttacher::attach_color(const glm::vec3& color) const
{
  _registry->emplace_or_replace<Color>(_entity, color);
}

void
ComponetsAttacher::attach_tutorial_button_value(Control::Action action) const
{
  _registry->emplace_or_replace<TutorialButton>(_entity, action);
}

void
ComponetsAttacher::attach_weapon(const EntityParamsActor& actor_params) const
{
  if (!actor_params.weapon_id.has_value()) {
    return;
  }
  auto gun_params = _entities->weapon(actor_params.weapon_id.value());
  auto bullet_model_path = _entities->model(gun_params.bullet_model_id).path;
  auto& gun = _registry->emplace_or_replace<Weapon>(_entity);
  gun.bullet_speed = gun_params.bullet_speed;
  gun.bullet_model_path = bullet_model_path;
  gun.spread = gun_params.spread;
  gun.shot_sound_path = gun_params.shot_sound_path;
  gun.lifetime = gun_params.lifetime;
  gun.fire_rate = gun_params.fire_rate;
}

void
ComponetsAttacher::attach_particles_emmiter_by_hit(
  const EntityParamsActor& actor_params) const
{
  if (!actor_params.hit_particles_id.has_value()) {
    return;
  }
  const auto particles_params = _entities->particles(
    actor_params.hit_particles_id.value());
  auto model_params = _entities->model(particles_params.model_id);
  auto scene = _scene; // The App should outlive everything in the game, so it
                       // should be safe enough
  ParticlesEmitter emitter { [scene, particles_params, model_params](
                               glm::vec3 position) {
    emit_particles(
      scene->state(), position, particles_params, model_params.path);
  } };
  _registry->emplace_or_replace<ParticlesEmitter>(_entity, emitter);
}

void
ComponetsAttacher::attach_debris_emmiter(
  const EntityParamsActor& actor_params) const
{
  if (!actor_params.debris_id.has_value()) {
    return;
  }
  auto& debris_id = actor_params.debris_id.value();
  auto model_path = _entities->model(debris_id).path;
  DebrisEmitter emitter { [model_path](entt::registry& registry,
                                       glm::vec3 position) {
    auto entity = ModelFactory::make_debris(registry, model_path);
    auto& transform = registry.get<Transform>(entity);
    transform.translate(position);
  } };
  _registry->emplace_or_replace<DebrisEmitter>(_entity, emitter);
}
