#pragma once
#include <functional>
#include <glm/vec3.hpp>
#include <string>

#include "entities_map.h"

import pln.control;
import pln.scene.iscene;

class ComponetsAttacher
{
private:
  std::reference_wrapper<const pln::scene::IScene> _scene;
  entt::entity _entity;
  std::reference_wrapper<const EntityParamsMap> _entities;

  ComponetsAttacher(std::reference_wrapper<const pln::scene::IScene> scene,
                    const entt::entity entity,
                    std::reference_wrapper<const EntityParamsMap> params_map);

public:
  void operator()(const EntityParamsActor& params) const;
  void operator()(const EntityParamsDirectionalLight& params) const;
  void operator()(const EntityParamsModel& params) const;
  void operator()(const EntityParamsParticles& params) const;
  void operator()(const EntityParamsPointLight& params) const;
  void operator()(const EntityParamsText& params) const;
  void operator()(const EntityParamsTutorialButton& params) const;
  void operator()(const EntityParamsWeapon& params) const;

  void attach(const EntityParams& params) const;
  void attach_velocity(const VelocityParams& velocity) const;
  void attach_opaque(bool is_opaque) const;
  void attach_rotation_speed(std::optional<float> rotation_speed) const;
  void attach_direction(const glm::vec3& direction) const;
  void attach_lives(int lives) const;
  void attach_color(const glm::vec3& color) const;
  void attach_text(const std::string& text) const;
  void attach_tutorial_button_value(pln::control::Action action) const;
  void attach_weapon(const EntityParamsActor& actor_params) const;
  void attach_particles_emmiter_by_hit(
    const EntityParamsActor& actor_params) const;
  void attach_debris_emmiter(const EntityParamsActor& actor_params) const;

  static void attach(const pln::scene::IScene& scene,
                     const entt::entity entity,
                     const EntityParamsMap& params_map,
                     const EntityParams& params);
};
