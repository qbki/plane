#pragma once
#include <entt/core/type_traits.hpp>
#include <glm/detail/qualifier.hpp>
#include <glm/vec3.hpp>
#include <memory>

#include "mesh.h"
#include "texture.h"
#include "utils.h"


enum class EnemyStateEnum {
  HUNTING = 0,
  SINKING,
  INACTIVE,
};

using InitialPosition = NewType<glm::vec3, struct InitialPositionTag>;
using Lifetime = NewType<float, struct LifeTimeTag>;
using LifetimeMax = NewType<float, struct LifeTimeMaxTag>;
using MeshPointer = std::shared_ptr<Mesh>;
using Position = NewType<glm::vec3, struct PositionTag>;
using Range = NewType<float, struct RangeTag>;
using Rotation = NewType<glm::vec3, struct RotationTag>;
using Speed = NewType<float, struct SpeedTag>;
using TexturePointer = std::shared_ptr<Texture>;

struct Available {};
struct Gravity {};
struct Opaque {};

struct EnemyKind {};
struct ParticleKind {};
struct PlayerKind {};
struct ProjectileKind {};

struct Velocity {
  float scalar_acceleration;
  float damping;
  glm::vec3 acceleration;
  glm::vec3 velocity;
  Velocity(float _acceleration, float _damping)
    : acceleration(glm::vec3(0.0)),
      velocity(glm::vec3(0.0)),
      scalar_acceleration(_acceleration),
      damping(_damping) {}
  Velocity(glm::vec3 _initial_velocity, float _damping)
    : acceleration(glm::vec3(0.0)),
      velocity(_initial_velocity),
      scalar_acceleration(0),
      damping(_damping) {}
};
