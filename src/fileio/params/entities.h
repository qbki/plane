#pragma once
#include <cstddef>
#include <filesystem> // IWYU pragma: export
#include <glm/vec3.hpp>
#include <optional> // IWYU pragma: export
#include <string>
#include <variant>

#include "src/control.h"

struct VelocityParams
{
  std::optional<float> acceleration = std::nullopt;
  std::optional<float> speed = std::nullopt;
  std::optional<float> damping = std::nullopt;
};

struct EntityParamsModel
{
  std::filesystem::path path;
  bool is_opaque;
};

struct EntityParamsActor
{
  VelocityParams speed;
  int lives;
  std::string model_id;
  std::optional<std::string> weapon_id = std::nullopt;
  std::optional<std::string> debris_id = std::nullopt;
  std::optional<std::string> hit_particles_id = std::nullopt;
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
