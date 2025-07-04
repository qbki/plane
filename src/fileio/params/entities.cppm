module;
#include <cstddef>
#include <filesystem> // IWYU pragma: export
#include <glm/vec3.hpp>
#include <optional> // IWYU pragma: export
#include <string>
#include <variant>

export module pln.fileio.params.entities;

import pln.control;

namespace pln::fileio::params {

export
struct VelocityParams
{
  std::optional<float> acceleration { std::nullopt };
  std::optional<float> speed { std::nullopt };
  std::optional<float> damping { std::nullopt };
};


export
struct EntityParamsModel
{
  std::filesystem::path path {};
  bool is_opaque { true };
};


export
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


export
struct EntityParamsPointLight
{
  glm::vec3 color { 0, 0, 0 };
};


export
struct EntityParamsDirectionalLight
{
  glm::vec3 color { 0, 0, 0 };
  glm::vec3 direction { 0, 0, -1 };
};


export
struct EntityParamsParticles
{
  std::string model_id { "" };
  float lifetime { 0 };
  size_t quantity { 0 };
  float speed { 0 };
};


export
struct EntityParamsWeapon
{
  float bullet_speed { 0 };
  float fire_rate { 0 };
  float lifetime { 0 };
  float spread { 0 };
  std::string bullet_model_id { "" };
  std::optional<std::filesystem::path> shot_sound_path {};
};


export
struct EntityParamsTutorialButton
{
  std::string model_id { "" };
  pln::control::Action button { pln::control::Action::UNKNOWN };
};


export
struct EntityParamsText
{
  glm::vec3 color { 0, 0, 0 };
  std::string text_id { "" };
  int size { 1 };
  float width { 1 };
  float height { 1 };
};


export
using EntityParams = std::variant<EntityParamsActor,
                                  EntityParamsDirectionalLight,
                                  EntityParamsModel,
                                  EntityParamsParticles,
                                  EntityParamsPointLight,
                                  EntityParamsText,
                                  EntityParamsTutorialButton,
                                  EntityParamsWeapon>;
}
