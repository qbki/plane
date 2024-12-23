#include <variant>

#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/components/turret_rotation.h"
#include "src/components/weapon.h"
#include "src/consts.h"
#include "src/game_state/factory.h"
#include "src/gui/types.h"
#include "src/scene/scene.h"
#include "src/services.h"
#include "src/utils/color.h"
#include "src/utils/noop.h"

#include "components_attacher.h"
#include "emitters.h"
#include "entities_map.h"

ComponetsAttacher::ComponetsAttacher(Scene* scene,
                                     const entt::entity entity,
                                     const EntityParamsMap* params_map)
  : _scene(scene)
  , _entity(entity)
  , _entities(params_map)
{
}

void
ComponetsAttacher::operator()(const EntityParamsActor& params) const
{
  attach_velocity(params.speed);
  attach_rotation_speed(params.rotation_speed);
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
ComponetsAttacher::operator()(const EntityParamsText& params) const
{
  auto t = Services::app().translate_fn();
  auto text = t(params.text_id);
  attach_color(params.color);
  attach_text(text);
  auto [font, transform, color] = _scene->state()
                                    .registry()
                                    .get<GUI::FontPtr, Transform, Core::Color>(
                                      _entity);

  font = Services::cache().get_font(Services::theme().font_path, params.size);
  auto [text_width, text_height] = font->calculate_size(text);
  auto ratio = static_cast<double>(text_width)
               / static_cast<double>(text_height);
  float height = 0;
  float width = 0;

  if (params.width.has_value() && params.height.has_value()) {
    width = params.width.value();
    height = params.height.value();
  } else if (params.width.has_value()) {
    width = params.width.value();
    height = width / static_cast<float>(ratio);
  } else if (params.height.has_value()) {
    height = params.height.value();
    width = static_cast<float>(ratio) * height;
  } else {
    height = 1;
    width = static_cast<float>(ratio) * height;
  }

  transform.scale({ width * HALF, height * HALF, 1 });
  color = params.color;
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
ComponetsAttacher::attach_lives(int lives) const
{
  Lives value {
    .max = lives,
    .value = lives,
  };
  _scene->state().registry().emplace_or_replace<Lives>(_entity, value);
}

void
ComponetsAttacher::attach_velocity(const VelocityParams& velocity) const
{
  auto& registry = _scene->state().registry();
  if (velocity.acceleration.has_value()) {
    registry.emplace_or_replace<AccelerationScalar>(
      _entity, velocity.acceleration.value());
  }
  if (velocity.speed.has_value()) {
    registry.emplace_or_replace<Speed>(_entity, velocity.speed.value());
  }
  if (velocity.damping.has_value()) {
    registry.emplace_or_replace<VelocityDamping>(_entity,
                                                 velocity.damping.value());
  }
}

void
ComponetsAttacher::attach_rotation_speed(
  std::optional<float> rotation_speed) const
{
  if (rotation_speed.has_value()) {
    auto& registry = _scene->state().registry();
    TurretRotation turret_rotation;
    turret_rotation.speed(rotation_speed.value());
    registry.emplace_or_replace<TurretRotation>(_entity, turret_rotation);
  }
}

void
ComponetsAttacher::attach_opaque(bool is_opaque) const
{
  auto& registry = _scene->state().registry();
  if (is_opaque) {
    registry.emplace_or_replace<Opaque>(_entity);
  }
}

void
ComponetsAttacher::attach_direction(const glm::vec3& direction) const
{
  auto& registry = _scene->state().registry();
  registry.emplace_or_replace<Direction>(_entity, direction);
}

void
ComponetsAttacher::attach_color(const glm::vec3& color) const
{
  auto& registry = _scene->state().registry();
  registry.emplace_or_replace<Color>(_entity, color);
}

void
ComponetsAttacher::attach_text(const std::string& text) const
{
  auto& registry = _scene->state().registry();
  registry.emplace_or_replace<Text>(_entity, text);
}

void
ComponetsAttacher::attach_tutorial_button_value(Control::Action action) const
{
  auto& registry = _scene->state().registry();
  registry.emplace_or_replace<TutorialButton>(_entity, action);
}

void
ComponetsAttacher::attach_weapon(const EntityParamsActor& actor_params) const
{
  auto& registry = _scene->state().registry();
  if (!actor_params.weapon_id.has_value()) {
    return;
  }
  auto gun_params = _entities->weapon(actor_params.weapon_id.value());
  auto bullet_model_path = _entities->model(gun_params.bullet_model_id).path;
  auto& gun = registry.emplace_or_replace<Weapon>(_entity);
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
  auto& registry = _scene->state().registry();
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
  registry.emplace_or_replace<ParticlesEmitter>(_entity, emitter);
}

void
ComponetsAttacher::attach_debris_emmiter(
  const EntityParamsActor& actor_params) const
{
  auto& registry = _scene->state().shared_registry();
  if (!actor_params.debris_id.has_value()) {
    return;
  }
  auto& debris_id = actor_params.debris_id.value();
  auto model_path = _entities->model(debris_id).path;
  DebrisEmitter emitter { [model_path](
                            std::shared_ptr<entt::registry>& registry,
                            glm::vec3 position) {
    auto entity = ModelFactory::make_debris(registry, model_path);
    auto& transform = registry->get<Transform>(entity);
    transform.translate(position);
  } };
  registry->emplace_or_replace<DebrisEmitter>(_entity, emitter);
}
