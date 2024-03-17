#pragma once
#include <glm/vec3.hpp>
#include <string>
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

struct SquareStrategyParams
{
  int width;
  int height;
  glm::vec3 center;
  std::vector<std::string> entity_ids;
};

struct RoundStrategyParams
{
  int radius;
  glm::vec3 center;
  std::vector<std::string> entity_ids;
};
