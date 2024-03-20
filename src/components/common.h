#pragma once
#include <entt/entity/fwd.hpp>
#include <functional> // IWYU pragma: export
#include <glm/gtc/constants.hpp>
#include <glm/vec3.hpp>
#include <memory>

#include "src/control.h"
#include "src/mesh.h"
#include "src/utils/types.h" // IWYU pragma: export

enum class EnemyStateEnum
{
  HUNTING = 0,
  SINKING,
  INACTIVE,
};

using Color = NewType<glm::vec3, struct ColorTag>;
using Direction = NewType<glm::vec3, struct DirectionTag>;
using InitialPosition = NewType<glm::vec3, struct InitialPositionTag>;
using Lifetime = NewType<float, struct LifeTimeTag>;
using LifetimeMax = NewType<float, struct LifeTimeMaxTag>;
using MeshPointer = std::shared_ptr<Mesh>;
using Range = NewType<float, struct RangeTag>;
using Speed = NewType<float, struct SpeedTag>;
using TutorialButton = NewType<Control::Action, struct TutorialButtonTag>;

struct Available
{};
struct Gravity
{};
struct Opaque
{};

struct DirectionalLightKind
{};
struct EnemyKind
{};
struct ParticleKind
{};
struct PlayerKind
{};
struct PointLightKind
{};
struct ProjectileKind
{};
struct TutorialButtonKind
{};

using ParticlesEmitter = std::function<void(glm::vec3 position)>;
using ProjectileEmitter = std::function<void()>;

struct PointLightParams
{
  float constant = 1.0;
  float linear = 1.0;
  float quadratic = 1.0;
};
