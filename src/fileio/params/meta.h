#pragma once
#include <glm/vec3.hpp>

struct CameraParams
{
  glm::vec3 position { 0, 0, 0 };
};

struct BoundaryParams
{
  glm::vec3 min { 0, 0, 0 };
  glm::vec3 max { 0, 0, 0 };
};

struct LevelMetaParams
{
  CameraParams camera;
  BoundaryParams boundaries;
};
