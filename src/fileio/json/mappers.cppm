module;
#include <format>
#include <glm/vec3.hpp>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <optional>
#include <vector>

export module pln.fileio.json.mappers;

import pln.control;
import pln.fileio.params.entities;
import pln.fileio.params.entities_map;
import pln.fileio.params.entities_validators;
import pln.fileio.params.meta;
import pln.fileio.params.strategies;
import pln.fileio.params.theme;
import pln.services.app;
import pln.utils.crash;

using namespace pln::fileio::params;

template<typename T>
void
set_optional(std::optional<T>& value,
             const nlohmann::json& json_obj,
             const std::string& key)
{
  if (json_obj.contains(key)) {
    value = std::make_optional(json_obj.at(key).get<T>());
  }
}

// cppcheck-suppress unknownMacro
NLOHMANN_JSON_NAMESPACE_BEGIN

template<>
struct adl_serializer<BehaviourEnum>
{
  static void from_json(const nlohmann::json& json_obj, BehaviourEnum& value)
  {
    auto behaviour = json_obj.get<std::string>();
    if (behaviour == "enemy") {
      value = BehaviourEnum::ENEMY;
    } else if (behaviour == "light") {
      value = BehaviourEnum::LIGHT;
    } else if (behaviour == "player") {
      value = BehaviourEnum::PLAYER;
    } else if (behaviour == "static") {
      value = BehaviourEnum::STATIC;
    } else if (behaviour == "tutorial_button") {
      value = BehaviourEnum::TUTORIAL_BUTTON;
    } else {
      pln::utils::crash(std::format("Unknown behaviour: {}", behaviour));
    }
  }
};


template<>
struct adl_serializer<pln::control::Action>
{
  static void from_json(const nlohmann::json& json_obj, pln::control::Action& value)
  {
    auto action = json_obj.get<std::string>();
    if (action == "up") {
      value = pln::control::Action::UP;
    } else if (action == "left") {
      value = pln::control::Action::LEFT;
    } else if (action == "right") {
      value = pln::control::Action::RIGHT;
    } else if (action == "down") {
      value = pln::control::Action::DOWN;
    } else if (action == "shooting") {
      value = pln::control::Action::SHOOTING;
    } else {
      pln::utils::crash(std::format("Unknown action: {}", action));
    }
  }
};


template<>
struct adl_serializer<glm::vec3>
{
  static void from_json(const nlohmann::json& json_obj, glm::vec3& value)
  {
    json_obj.at(0).get_to(value.x);
    json_obj.at(1).get_to(value.y);
    json_obj.at(2).get_to(value.z);
  }
};


template<>
struct adl_serializer<std::vector<glm::vec3>>
{
  static void from_json(const nlohmann::json& json_obj,
                        std::vector<glm::vec3>& value)
  {
    const auto length = json_obj.size() / 3;
    for (unsigned int i = 0; i < length; i++) {
      glm::vec3 point;
      json_obj.at(i * 3 + 0).get_to(point.x);
      json_obj.at(i * 3 + 1).get_to(point.y);
      json_obj.at(i * 3 + 2).get_to(point.z);
      value.push_back(point);
    }
  }
};


template<>
struct adl_serializer<CameraParams>
{
  static void from_json(const nlohmann::json& json, CameraParams& value)
  {
    json.at("position").get_to(value.position);
  }
};


template<>
struct adl_serializer<BoundaryParams>
{
  static void from_json(const nlohmann::json& json, BoundaryParams& value)
  {
    json.at("min").get_to(value.min);
    json.at("max").get_to(value.max);
  }
};


template<>
struct adl_serializer<LevelMetaParams>
{
  static void from_json(const nlohmann::json& json, LevelMetaParams& value)
  {
    json.at("camera").get_to(value.camera);
    json.at("boundaries").get_to(value.boundaries);
  }
};


template<>
struct adl_serializer<ThemeParams>
{
  static void from_json(const nlohmann::json& json, ThemeParams& value)
  {
    json.at("font-source").get_to(value.font_source);
    json.at("font-family").get_to(value.font_family);
  }
};


template<>
struct adl_serializer<VelocityParams>
{
  static void from_json(const nlohmann::json& json_obj, VelocityParams& value)
  {
    if (json_obj.is_number()) {
      value.speed = json_obj.get<double>();
      return;
    }
    auto get = [&](const std::string& name) {
      return json_obj.contains(name)
               ? std::make_optional(json_obj.at(name).get<float>())
               : std::nullopt;
    };
    value.acceleration = get("acceleration");
    value.speed = get("speed");
    value.damping = get("damping");
  }
};


template<>
struct adl_serializer<PositionStrategy>
{
  static void from_json(const nlohmann::json& json_obj, PositionStrategy& value)
  {
    auto kind = json_obj.at("kind").get<std::string>();
    if (kind == "void") {
      PositionStrategyVoid strategy {};
      json_obj.at("behaviour").get_to(strategy.behaviour);
      json_obj.at("entity_id").get_to(strategy.entity_id);
      value = strategy;
    } else if (kind == "round") {
      PositionStrategyRound strategy {};
      json_obj.at("behaviour").get_to(strategy.behaviour);
      json_obj.at("radius").get_to(strategy.radius);
      json_obj.at("center").get_to(strategy.center);
      json_obj.at("entity_ids").get_to(strategy.entity_ids);
      value = strategy;
    } else if (kind == "square") {
      PositionStrategySquare strategy {};
      json_obj.at("behaviour").get_to(strategy.behaviour);
      json_obj.at("width").get_to(strategy.width);
      json_obj.at("height").get_to(strategy.height);
      json_obj.at("center").get_to(strategy.center);
      json_obj.at("entity_ids").get_to(strategy.entity_ids);
      value = strategy;
    } else if (kind == "single") {
      PositionStrategySingle strategy {};
      json_obj.at("behaviour").get_to(strategy.behaviour);
      json_obj.at("position").get_to(strategy.position);
      json_obj.at("entity_id").get_to(strategy.entity_id);
      value = strategy;
    } else if (kind == "many") {
      PositionStrategyMany strategy {};
      json_obj.at("behaviour").get_to(strategy.behaviour);
      json_obj.at("positions").get_to(strategy.positions);
      json_obj.at("entity_id").get_to(strategy.entity_id);
      value = strategy;
    } else {
      pln::utils::crash(std::format("Unknown strategy: {}", kind));
    }
  }
};


template<>
struct adl_serializer<EntityParams>
{
  static void from_json(const nlohmann::json& json_obj, EntityParams& value)
  {
    auto kind = json_obj.at("kind").get<std::string>();
    if (kind == "model") {
      EntityParamsModel params {};
      json_obj.at("path").get_to(params.path);
      json_obj.at("is_opaque").get_to(params.is_opaque);
      value = params;
    } else if (kind == "particles") {
      EntityParamsParticles params {};
      json_obj.at("lifetime").get_to(params.lifetime);
      json_obj.at("model_id").get_to(params.model_id);
      json_obj.at("quantity").get_to(params.quantity);
      json_obj.at("speed").get_to(params.speed);
      value = params;
    } else if (kind == "actor") {
      EntityParamsActor params {};
      json_obj.at("model_id").get_to(params.model_id);
      json_obj.at("speed").get_to(params.speed);
      json_obj.at("lives").get_to(params.lives);
      set_optional(params.debris_id, json_obj, "debris_id");
      set_optional(params.hit_particles_id, json_obj, "hit_particles_id");
      set_optional(params.rotation_speed, json_obj, "rotation_speed");
      set_optional(params.weapon_id, json_obj, "weapon_id");
      value = params;
    } else if (kind == "weapon") {
      EntityParamsWeapon params {};
      json_obj.at("bullet_model_id").get_to(params.bullet_model_id);
      json_obj.at("bullet_speed").get_to(params.bullet_speed);
      json_obj.at("lifetime").get_to(params.lifetime);
      json_obj.at("spread").get_to(params.spread);
      json_obj.at("fire_rate").get_to(params.fire_rate);

      std::optional<std::string> shot_sound_path;
      set_optional(shot_sound_path, json_obj, "shot_sound_path");
      if (shot_sound_path.has_value()) {
        auto assets_dir = pln::services::app().assets_dir();
        params.shot_sound_path = assets_dir / shot_sound_path.value();
      }

      value = params;
    } else if (kind == "point_light") {
      EntityParamsPointLight params {};
      json_obj.at("color").get_to(params.color);
      value = params;
    } else if (kind == "directional_light") {
      EntityParamsDirectionalLight params {};
      json_obj.at("color").get_to(params.color);
      json_obj.at("direction").get_to(params.direction);
      value = params;
    } else if (kind == "tutorial_button") {
      EntityParamsTutorialButton params {};
      json_obj.at("model_id").get_to(params.model_id);
      json_obj.at("button").get_to(params.button);
      value = params;
    } else if (kind == "text") {
      EntityParamsText params {};
      json_obj.at("color").get_to(params.color);
      json_obj.at("text_id").get_to(params.text_id);
      json_obj.at("size").get_to(params.size);
      json_obj.at("width").get_to(params.width);
      json_obj.at("height").get_to(params.width);
      validate(params);
      value = params;
    } else {
      pln::utils::crash(std::format("Unknown entity: {}", kind));
    }
  };
};


template<>
struct adl_serializer<EntityParamsMap>
{
  static void from_json(const nlohmann::json& json_obj, EntityParamsMap& value)
  {
    value.set(json_obj.get<EntityParamsMap::Mapping>());
  }
};

NLOHMANN_JSON_NAMESPACE_END
