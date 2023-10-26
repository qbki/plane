#pragma once
#include <entt/core/type_traits.hpp>
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
using TexturePointer = std::shared_ptr<Texture>;

struct Opaque {};
struct Available {};

struct EnemyKind {};
struct ParticleKind {};
struct PlayerKind {};
struct ProjectileKind {};
