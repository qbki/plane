#pragma once

#include <cstddef>
#include <filesystem> // IWYU pragma: export
#include <glm/vec3.hpp>
#include <optional> // IWYU pragma: export
#include <string>
#include <variant>

import control;

struct VelocityParams
{
  std::optional<float> acceleration { std::nullopt };
  std::optional<float> speed { std::nullopt };
  std::optional<float> damping { std::nullopt };
};

struct EntityParamsModel
{
  std::filesystem::path path {};
  bool is_opaque { true };
};

struct EntityParamsActor
{
  VelocityParams speed {};
  int lives { 0 };
  std::string model_id { "" };
  std::optional<float> rotation_speed { 0 };
  std::optional<std::string> weapon_id { std::nullopt };
  std::optional<std::string> debris_id { std::nullopt };
  std::optional<std::string> hit_particles_id { std::nullopt };
};

struct EntityParamsPointLight
{
  glm::vec3 color { 0, 0, 0 };
};

struct EntityParamsDirectionalLight
{
  glm::vec3 color { 0, 0, 0 };
  glm::vec3 direction { 0, 0, -1 };
};

struct EntityParamsParticles
{
  std::string model_id { "" };
  float lifetime { 0 };
  size_t quantity { 0 };
  float speed { 0 };
};

struct EntityParamsWeapon
{
  float bullet_speed { 0 };
  float fire_rate { 0 };
  float lifetime { 0 };
  float spread { 0 };
  std::string bullet_model_id { "" };
  std::optional<std::filesystem::path> shot_sound_path {};
};

struct EntityParamsTutorialButton
{
  std::string model_id { "" };
  control::Action button { control::Action::UNKNOWN };
};

struct EntityParamsText
{
  glm::vec3 color { 0, 0, 0 };
  std::string text_id { "" };
  int size { 1 };
  float width { 1 };
  float height { 1 };
};

using EntityParams = std::variant<EntityParamsActor,
                                  EntityParamsDirectionalLight,
                                  EntityParamsModel,
                                  EntityParamsParticles,
                                  EntityParamsPointLight,
                                  EntityParamsText,
                                  EntityParamsTutorialButton,
                                  EntityParamsWeapon>;
