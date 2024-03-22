#pragma once
#include <format>
#include <glm/vec3.hpp>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <utility>

#include "src/loader/params.h"

// cppcheck-suppress unknownMacro
NLOHMANN_JSON_NAMESPACE_BEGIN
template<>
struct adl_serializer<glm::vec3>
{
  static void to_json(nlohmann::json&, const glm::vec3&)
  {
    throw std::runtime_error("Not implemented");
  }
  static void from_json(const nlohmann::json& json_obj, glm::vec3& value)
  {
    json_obj.at(0).get_to(value.x);
    json_obj.at(1).get_to(value.y);
    json_obj.at(2).get_to(value.z);
  }
};
NLOHMANN_JSON_NAMESPACE_END

NLOHMANN_JSON_NAMESPACE_BEGIN
template<>
struct adl_serializer<VelocityParams>
{
  static void to_json(nlohmann::json&, const VelocityParams&)
  {
    throw std::runtime_error("Not implemented");
  }
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
NLOHMANN_JSON_NAMESPACE_END

NLOHMANN_JSON_NAMESPACE_BEGIN
template<>
struct adl_serializer<ProjectilesParams>
{
  static void to_json(nlohmann::json&, const ProjectilesParams&)
  {
    throw std::runtime_error("Not implemented");
  }
  static void from_json(const nlohmann::json& json_obj,
                        ProjectilesParams& value)
  {
    json_obj.at("projectile_id").get_to(value.projectile_id);
    json_obj.at("range").get_to(value.range);
    json_obj.at("velocity").get_to(value.velocity);
  }
};
NLOHMANN_JSON_NAMESPACE_END

NLOHMANN_JSON_NAMESPACE_BEGIN
template<>
struct adl_serializer<ParticlesParams>
{
  static void to_json(nlohmann::json&, const ParticlesParams&)
  {
    throw std::runtime_error("Not implemented");
  }
  static void from_json(const nlohmann::json& json_obj, ParticlesParams& value)
  {
    json_obj.at("lifetime").get_to(value.lifetime);
    json_obj.at("quantity").get_to(value.quantity);
    json_obj.at("velocity").get_to(value.velocity);
  }
};
NLOHMANN_JSON_NAMESPACE_END

NLOHMANN_JSON_NAMESPACE_BEGIN
template<>
struct adl_serializer<PositionStrategy>
{
  static void to_json(nlohmann::json&, const PositionStrategy&)
  {
    throw std::runtime_error("Not implemented");
  }
  static void from_json(const nlohmann::json& json_obj, PositionStrategy& value)
  {
    auto kind = json_obj.at("position_strategy").get<std::string>();
    if (kind == "void") {
      PositionStrategyVoid strategy{};
      json_obj.at("entity_id").get_to(strategy.entity_id);
      value = strategy;
    } else if (kind == "round") {
      PositionStrategyRound strategy{};
      json_obj.at("radius").get_to(strategy.radius);
      json_obj.at("center").get_to(strategy.center);
      json_obj.at("entity_ids").get_to(strategy.entity_ids);
      value = strategy;
    } else if (kind == "square") {
      PositionStrategySquare strategy{};
      json_obj.at("width").get_to(strategy.width);
      json_obj.at("height").get_to(strategy.height);
      json_obj.at("center").get_to(strategy.center);
      json_obj.at("entity_ids").get_to(strategy.entity_ids);
      value = strategy;
    } else if (kind == "single") {
      PositionStrategySingle strategy{};
      json_obj.at("position").get_to(strategy.position);
      json_obj.at("entity_id").get_to(strategy.entity_id);
      value = strategy;
    } else {
      std::runtime_error(std::format("Unknown strategy: {}", kind));
    }
  }
};
NLOHMANN_JSON_NAMESPACE_END

NLOHMANN_JSON_NAMESPACE_BEGIN
template<>
struct adl_serializer<BehaviourEnum>
{
  static void to_json(nlohmann::json&, const BehaviourEnum&)
  {
    throw std::runtime_error("Not implemented");
  }
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
    } else if (behaviour == "tutorial-button") {
      value = BehaviourEnum::TUTORIAL_BUTTON;
    } else {
      throw std::runtime_error(std::format("Unknown behaviour: {}", behaviour));
    }
  }
};
NLOHMANN_JSON_NAMESPACE_END
