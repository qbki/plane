#include <variant>

#include "src/components/common.h"
#include "src/components/velocity.h"
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
  attach_velocity(params.velocity);
  attach_particles_emmiter_by_hit(params);
  attach_projectile_emitter(params);
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
ComponetsAttacher::operator()(const EntityParamsGun&) const
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
ComponetsAttacher::attach_particles_emmiter_by_hit(
  const EntityParamsActor& actor_params) const
{
  if (!actor_params.hit_particles_id.has_value()) {
    return;
  }
  const auto particles_params =
    _entities->particles(actor_params.hit_particles_id.value());
  auto model_params = _entities->model(particles_params.model_id);
  auto scene = _scene; // The App should outlive everything in the game, so it
                       // should be safe enough
  ParticlesEmitter emitter{ [scene, particles_params, model_params](
                              glm::vec3 position) {
    emit_particles(
      scene->state(), position, particles_params, model_params.path);
  } };
  _registry->emplace_or_replace<ParticlesEmitter>(_entity, emitter);
}

void
ComponetsAttacher::attach_projectile_emitter(
  const EntityParamsActor& actor_params) const
{
  if (!actor_params.gun_id.has_value()) {
    _registry->emplace_or_replace<ShotSound>(_entity, std::nullopt);
    _registry->emplace_or_replace<ProjectileEmitter>(_entity, std::nullopt);
    return;
  }
  auto gun_params = _entities->gun(actor_params.gun_id.value());
  auto model_params = _entities->model(gun_params.model_id);
  auto scene = _scene; // The App should outlive everything in the game, so it
                       // should be safe enough
  ProjectileEmitter emitter{ [scene, gun_params, model_params](auto entity) {
    emit_projectile(scene->state(), entity, gun_params, model_params.path);
  } };
  _registry->emplace_or_replace<ShotSound>(_entity, gun_params.sound_shot);
  _registry->emplace_or_replace<ProjectileEmitter>(_entity, emitter);
}
