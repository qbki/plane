#pragma once
#include <filesystem> // IWYU pragma: export
#include <glm/vec3.hpp>
#include <optional> // IWYU pragma: export
#include <string>
#include <variant>
#include <vector>

#include "src/control.h"

struct VelocityParams
{
  float acceleration;
  float speed;
  float damping;
};

struct EntityParamsModel
{
  std::filesystem::path path;
  bool is_opaque;
};

struct EntityParamsActor
{
  VelocityParams velocity;
  std::string model_id;
  std::optional<std::string> gun_id;
  std::optional<std::string> hit_particles_id;
};

struct EntityParamsPointLight
{
  glm::vec3 color;
};

struct EntityParamsDirectionalLight
{
  glm::vec3 color;
  glm::vec3 direction;
};

struct EntityParamsParticles
{
  std::string model_id;
  float lifetime;
  size_t quantity;
  VelocityParams velocity;
};

struct EntityParamsGun
{
  std::string model_id;
  float range;
  VelocityParams velocity;
  std::filesystem::path sound_shot;
};

struct EntityParamsTutorialButton
{
  std::string model_id;
  Control::Action button;
};

using EntityParams = std::variant<EntityParamsActor,
                                  EntityParamsDirectionalLight,
                                  EntityParamsGun,
                                  EntityParamsModel,
                                  EntityParamsParticles,
                                  EntityParamsPointLight,
                                  EntityParamsTutorialButton>;
