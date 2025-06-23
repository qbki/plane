module;
#include <entt/entt.hpp>
#include <functional>
#include <glm/vec3.hpp>
#include <string>
#include <variant>

#include "entities.h"

export module pln.fileio.params.components_attacher;

import pln.components.common;
import pln.components.transform;
import pln.components.turret_rotation;
import pln.components.weapon;
import pln.control;
import pln.fileio.params.emitters;
import pln.fileio.params.entities_map;
import pln.fileio.params.strategies;
import pln.game_state.factory;
import pln.scene.iscene;
import pln.services.app;
import pln.utils.color;
import pln.utils.common;

using namespace pln::components;
using namespace pln::fileio::params;
using namespace pln::game_state;

namespace pln::fileio::params {

export
class ComponetsAttacher
{
private:
  std::reference_wrapper<const pln::scene::IScene> _scene;
  entt::entity _entity;
  std::reference_wrapper<const EntityParamsMap> _entities;

  ComponetsAttacher(
    std::reference_wrapper<const pln::scene::IScene> scene,
    const entt::entity entity,
    std::reference_wrapper<const EntityParamsMap> params_map)
    : _scene(scene)
    , _entity(entity)
    , _entities(params_map)
  {
  }


public:
  void
  operator()(const EntityParamsActor& params) const
  {
    attach_velocity(params.speed);
    attach_rotation_speed(params.rotation_speed);
    attach_particles_emmiter_by_hit(params);
    attach_debris_emmiter(params);
    attach_lives(params.lives);
    attach_weapon(params);
    auto model = _entities.get().model(params.model_id);
    (*this)(model);
  }

  void
  operator()(const EntityParamsDirectionalLight& params) const
  {
    attach_color(params.color);
    attach_direction(params.direction);
  }

  void
  operator()(const EntityParamsPointLight& params) const
  {
    attach_color(params.color);
  }

  void
  operator()(const EntityParamsText& params) const
  {
    auto t = pln::services::app().translate_fn();
    auto text = t(params.text_id);
    attach_color(params.color);
    attach_text(text);
    auto [transform, color] = _scene.get()
                                .state()
                                .registry()
                                .get<Transform, pln::utils::color::Color>(_entity);
    transform.scale({ params.width, params.height, 1 });
    color = params.color;
  }

  void
  operator()(const EntityParamsWeapon&) const
  {
    pln::utils::common::noop();
  }

  void
  operator()(const EntityParamsModel& params) const
  {
    attach_opaque(params.is_opaque);
  }

  void
  operator()(const EntityParamsParticles&) const
  {
  }

  void
  operator()(const EntityParamsTutorialButton& params) const
  {
    attach_tutorial_button_value(params.button);
    auto model = _entities.get().model(params.model_id);
    (*this)(model);
  }

  void
  attach(const EntityParams& params) const
  {
    std::visit(*this, params);
  }

  void
  attach_lives(int lives) const
  {
    Lives value {
      .max = lives,
      .value = lives,
    };
    _scene.get().state().registry().emplace_or_replace<Lives>(_entity, value);
  }

  void
  attach_velocity(const VelocityParams& velocity) const
  {
    auto& registry = _scene.get().state().registry();
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
  attach_rotation_speed(
    std::optional<float> rotation_speed) const
  {
    if (rotation_speed.has_value()) {
      auto& registry = _scene.get().state().registry();
      TurretRotation turret_rotation;
      turret_rotation.speed(rotation_speed.value());
      registry.emplace_or_replace<TurretRotation>(_entity, turret_rotation);
    }
  }

  void
  attach_opaque(bool is_opaque) const
  {
    auto& registry = _scene.get().state().registry();
    if (is_opaque) {
      registry.emplace_or_replace<Opaque>(_entity);
    }
  }

  void
  attach_direction(const glm::vec3& direction) const
  {
    auto& registry = _scene.get().state().registry();
    registry.emplace_or_replace<Direction>(_entity, direction);
  }

  void
  attach_color(const glm::vec3& color) const
  {
    auto& registry = _scene.get().state().registry();
    registry.emplace_or_replace<Color>(_entity, color);
  }

  void
  attach_text(const std::string& text) const
  {
    auto& registry = _scene.get().state().registry();
    registry.emplace_or_replace<Text>(_entity, text);
  }

  void
  attach_tutorial_button_value(pln::control::Action action) const
  {
    auto& registry = _scene.get().state().registry();
    registry.emplace_or_replace<TutorialButton>(_entity, action);
  }

  void
  attach_weapon(const EntityParamsActor& actor_params) const
  {
    auto& registry = _scene.get().state().registry();
    if (!actor_params.weapon_id.has_value()) {
      return;
    }
    auto gun_params = _entities.get().weapon(actor_params.weapon_id.value());
    auto
      bullet_model_path = _entities.get().model(gun_params.bullet_model_id).path;
    auto& gun = registry.emplace_or_replace<pln::components::Weapon>(_entity);
    gun.bullet_speed = gun_params.bullet_speed;
    gun.bullet_model_path = bullet_model_path;
    gun.spread = gun_params.spread;
    gun.shot_sound_path = gun_params.shot_sound_path;
    gun.lifetime = gun_params.lifetime;
    gun.fire_rate = gun_params.fire_rate;
  }

  void
  attach_particles_emmiter_by_hit(
    const EntityParamsActor& actor_params) const
  {
    auto registry = _scene.get().state().shared_registry();
    if (!actor_params.hit_particles_id.has_value()) {
      return;
    }
    const auto particles_params = _entities.get().particles(
      actor_params.hit_particles_id.value());
    auto model_params = _entities.get().model(particles_params.model_id);
    ParticlesEmitter emitter { [registry, particles_params, model_params](
                                 glm::vec3 position) {
      emit_particles(registry, position, particles_params, model_params.path);
    } };
    registry->emplace_or_replace<ParticlesEmitter>(_entity, emitter);
  }

  void
  attach_debris_emmiter(
    const EntityParamsActor& actor_params) const
  {
    auto& registry = _scene.get().state().shared_registry();
    if (!actor_params.debris_id.has_value()) {
      return;
    }
    auto& debris_id = actor_params.debris_id.value();
    auto model_path = _entities.get().model(debris_id).path;
    DebrisEmitter emitter { [model_path](
                              std::shared_ptr<entt::registry>& registry,
                              glm::vec3 position) {
      auto entity = factory::make_debris(registry, model_path);
      auto& transform = registry->get<Transform>(entity);
      transform.translate(position);
    } };
    registry->emplace_or_replace<DebrisEmitter>(_entity, emitter);
  }

  static void
  attach(const pln::scene::IScene& scene,
         const entt::entity entity,
         const EntityParamsMap& params_map,
         const EntityParams& params)
  {
    ComponetsAttacher(scene, entity, params_map).attach(params);
  }
};

}
