#pragma once
#include <format>
#include <glm/vec3.hpp>
#include <nlohmann/json.hpp>
#include <optional>
#include <stdexcept>
#include <vector>

#include "src/consts.h"
#include "src/control.h"
#include "src/loader/params.h"

template<typename T>
void
set_optional(std::optional<T>& value,
             const nlohmann::json& json_obj,
             const std::string& key)
{
  if (json_obj.contains(key)) {
    value = make_optional(json_obj.at(key).get<T>());
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
      throw std::runtime_error(std::format("Unknown behaviour: {}", behaviour));
    }
  }
};

template<>
struct adl_serializer<Control::Action>
{
  static void from_json(const nlohmann::json& json_obj, Control::Action& value)
  {
    auto action = json_obj.get<std::string>();
    if (action == "up") {
      value = Control::Action::UP;
    } else if (action == "left") {
      value = Control::Action::LEFT;
    } else if (action == "right") {
      value = Control::Action::RIGHT;
    } else if (action == "down") {
      value = Control::Action::DOWN;
    } else if (action == "shooting") {
      value = Control::Action::SHOOTING;
    } else {
      throw std::runtime_error(std::format("Unknown action: {}", action));
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
  static void from_json(const nlohmann::json& json_obj, CameraParams& value)
  {
    json_obj.at("position").get_to(value.position);
  }
};

template<>
struct adl_serializer<VelocityParams>
{
  static void from_json(const nlohmann::json& json_obj, VelocityParams& value)
  {
    value.acceleration = json_obj.contains("acceleration")
                           ? json_obj.at("acceleration").get<float>()
                           : 0;
    value.speed =
      json_obj.contains("speed") ? json_obj.at("speed").get<float>() : 0;
    value.damping = json_obj.at("damping");
  }
};

template<>
struct adl_serializer<PositionStrategy>
{
  static void from_json(const nlohmann::json& json_obj, PositionStrategy& value)
  {
    auto kind = json_obj.at("kind").get<std::string>();
    if (kind == "void") {
      PositionStrategyVoid strategy{};
      json_obj.at("behaviour").get_to(strategy.behaviour);
      json_obj.at("entity_id").get_to(strategy.entity_id);
      value = strategy;
    } else if (kind == "round") {
      PositionStrategyRound strategy{};
      json_obj.at("behaviour").get_to(strategy.behaviour);
      json_obj.at("radius").get_to(strategy.radius);
      json_obj.at("center").get_to(strategy.center);
      json_obj.at("entity_ids").get_to(strategy.entity_ids);
      value = strategy;
    } else if (kind == "square") {
      PositionStrategySquare strategy{};
      json_obj.at("behaviour").get_to(strategy.behaviour);
      json_obj.at("width").get_to(strategy.width);
      json_obj.at("height").get_to(strategy.height);
      json_obj.at("center").get_to(strategy.center);
      json_obj.at("entity_ids").get_to(strategy.entity_ids);
      value = strategy;
    } else if (kind == "single") {
      PositionStrategySingle strategy{};
      json_obj.at("behaviour").get_to(strategy.behaviour);
      json_obj.at("position").get_to(strategy.position);
      json_obj.at("entity_id").get_to(strategy.entity_id);
      value = strategy;
    } else if (kind == "many") {
      PositionStrategyMany strategy{};
      json_obj.at("behaviour").get_to(strategy.behaviour);
      json_obj.at("positions").get_to(strategy.positions);
      json_obj.at("entity_id").get_to(strategy.entity_id);
      value = strategy;
    } else {
      throw std::runtime_error(std::format("Unknown strategy: {}", kind));
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
      EntityParamsModel params{};
      json_obj.at("path").get_to(params.path);
      json_obj.at("is_opaque").get_to(params.is_opaque);
      value = params;
    } else if (kind == "particles") {
      EntityParamsParticles params{};
      json_obj.at("lifetime").get_to(params.lifetime);
      json_obj.at("model_id").get_to(params.model_id);
      json_obj.at("quantity").get_to(params.quantity);
      json_obj.at("velocity").get_to(params.velocity);
      value = params;
    } else if (kind == "actor") {
      EntityParamsActor params{};
      json_obj.at("model_id").get_to(params.model_id);
      json_obj.at("velocity").get_to(params.velocity);
      set_optional(params.gun_id, json_obj, "gun_id");
      set_optional(params.debris_id, json_obj, "debris_id");
      set_optional(params.hit_particles_id, json_obj, "hit_particles_id");
      value = params;
    } else if (kind == "gun") {
      EntityParamsGun params{};
      std::string sound_shot_path;
      json_obj.at("model_id").get_to(params.model_id);
      json_obj.at("range").get_to(params.range);
      json_obj.at("velocity").get_to(params.velocity);
      json_obj.at("sound_shot").get_to(sound_shot_path);
      params.sound_shot = ASSETS_DIR / sound_shot_path;
      value = params;
    } else if (kind == "point_light") {
      EntityParamsPointLight params{};
      json_obj.at("color").get_to(params.color);
      value = params;
    } else if (kind == "directional_light") {
      EntityParamsDirectionalLight params{};
      json_obj.at("color").get_to(params.color);
      json_obj.at("direction").get_to(params.direction);
      value = params;
    } else if (kind == "tutorial_button") {
      EntityParamsTutorialButton params{};
      json_obj.at("model_id").get_to(params.model_id);
      json_obj.at("button").get_to(params.button);
      value = params;
    } else {
      throw std::runtime_error(std::format("Unknown entity: {}", kind));
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
