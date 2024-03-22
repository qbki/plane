#pragma once
#include <glm/vec3.hpp>
#include <string>
#include <variant>
#include <vector>

struct VelocityParams
{
  float acceleration;
  float speed;
  float damping;
};

struct ParticlesParams
{
  unsigned int quantity{};
  VelocityParams velocity{};
  float lifetime{};
};

struct ProjectilesParams
{
  std::string projectile_id;
  VelocityParams velocity;
  float range;
};

struct PositionStrategyUndefined
{};

struct PositionStrategyRound
{
  int radius;
  glm::vec3 center;
  std::vector<std::string> entity_ids;
};

struct PositionStrategySingle
{
  glm::vec3 position;
  std::string entity_id;
};

struct PositionStrategySquare
{
  glm::vec3 center;
  int width;
  int height;
  std::vector<std::string> entity_ids;
};

struct PositionStrategyVoid
{
  std::string entity_id;
};

using PositionStrategy = std::variant<PositionStrategyUndefined,
                                      PositionStrategyRound,
                                      PositionStrategySingle,
                                      PositionStrategySquare,
                                      PositionStrategyVoid>;

enum class BehaviourEnum : char
{
  ENEMY = 0,
  LIGHT,
  PLAYER,
  STATIC,
  TUTORIAL_BUTTON,
};
