#pragma once
#include <glm/vec3.hpp>
#include <string>
#include <variant>
#include <vector>

enum class BehaviourEnum : char
{
  UNKNOWN = 0,
  ENEMY,
  LIGHT,
  PLAYER,
  STATIC,
  TUTORIAL_BUTTON,
};

struct PositionStrategyUndefined
{};

struct PositionStrategyRound
{
  BehaviourEnum behaviour;
  int radius;
  glm::vec3 center;
  std::vector<std::string> entity_ids;
};

struct PositionStrategySingle
{
  BehaviourEnum behaviour;
  glm::vec3 position;
  std::string entity_id;
};

struct PositionStrategyMany
{
  BehaviourEnum behaviour;
  std::vector<glm::vec3> positions;
  std::string entity_id;
};

struct PositionStrategySquare
{
  BehaviourEnum behaviour;
  glm::vec3 center;
  int width;
  int height;
  std::vector<std::string> entity_ids;
};

struct PositionStrategyVoid
{
  BehaviourEnum behaviour; // Supported only BehaviourEnum::LIGHT
  std::string entity_id;
};

using PositionStrategy = std::variant<PositionStrategyUndefined,
                                      PositionStrategyRound,
                                      PositionStrategySingle,
                                      PositionStrategyMany,
                                      PositionStrategySquare,
                                      PositionStrategyVoid>;

BehaviourEnum
get_behaviour(const PositionStrategy& strategy);
