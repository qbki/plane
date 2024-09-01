#pragma once
#include <filesystem> // IWYU pragma: export
#include <glm/vec3.hpp>
#include <optional> // IWYU pragma: export
#include <string>
#include <variant>

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
  float speed;
  std::string model_id;
  std::optional<std::string> weapon_id;
  std::optional<std::string> debris_id;
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
  float speed;
};

struct EntityParamsWeapon
{
  float bullet_speed;
  float fire_rate;
  float lifetime;
  float range;
  float spread;
  std::string bullet_model_id;
  std::filesystem::path shot_sound_path;
};

struct EntityParamsTutorialButton
{
  std::string model_id;
  Control::Action button;
};

using EntityParams = std::variant<EntityParamsActor,
                                  EntityParamsDirectionalLight,
                                  EntityParamsWeapon,
                                  EntityParamsModel,
                                  EntityParamsParticles,
                                  EntityParamsPointLight,
                                  EntityParamsTutorialButton>;
