#pragma once
#include <glm/vec3.hpp>

#include "src/app/app.h"

#include "entities_map.h"

class ComponetsAttacher
{
private:
  const App* _app;
  entt::entity _entity;
  entt::registry* _registry;
  const EntityParamsMap* _entities;

public:
  ComponetsAttacher(const App* app,
                    const entt::entity entity,
                    const EntityParamsMap* params_map);
  void operator()(const EntityParamsActor& params) const;
  void operator()(const EntityParamsDirectionalLight& params) const;
  void operator()(const EntityParamsGun& params) const;
  void operator()(const EntityParamsModel& params) const;
  void operator()(const EntityParamsParticles& params) const;
  void operator()(const EntityParamsPointLight& params) const;
  void operator()(const EntityParamsTutorialButton& params) const;

  void attach(const EntityParams& params) const;
  void attach_velocity(const VelocityParams& velocity) const;
  void attach_opaque(bool is_opaque) const;
  void attach_direction(const glm::vec3& direction) const;
  void attach_color(const glm::vec3& color) const;
  void attach_tutorial_button_value(Control::Action action) const;
  void attach_particles_emmiter_by_hit(
    const EntityParamsActor& actor_params) const;
  void attach_projectile_emmiter(const EntityParamsActor& actor_params) const;
};
