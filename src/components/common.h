#pragma once
#include <entt/entity/fwd.hpp>
#include <functional> // IWYU pragma: export
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <optional> // IWYU pragma: export
#include <vector>

#include "src/mesh.h" // IWYU pragma: export
#include "src/texture.h"
#include "src/utils/types.h" // IWYU pragma: export

import control;

enum class EnemyStateEnum
{
  HUNTING = 0,
  INACTIVE,
};

using Children = NewType<std::vector<entt::entity>, struct ChildrenTag>;
using Color = NewType<glm::vec3, struct ColorTag>;
using Direction = NewType<glm::vec3, struct DirectionTag>;
using InitialPosition = NewType<glm::vec3, struct InitialPositionTag>;
using IsDirty = NewType<bool, struct IsDirtyTag>;
using Lifetime = NewType<float, struct LifeTimeTag>;
using LifetimeMax = NewType<float, struct LifeTimeMaxTag>;
using MeshPointer = std::shared_ptr<Mesh>;
using Owner = NewType<entt::entity, struct OwnerTag>;
using Parent = NewType<std::optional<entt::entity>, struct ParentTag>;
using ShotSound = NewType<std::optional<std::string>, struct ShotSoundTag>;
using Text = NewType<std::string, struct TextTag>;
using TexturePointer = std::shared_ptr<Texture>;
using TutorialButton = NewType<pln::control::Action, struct TutorialButtonTag>;

using Acceleration = NewType<glm::vec3, struct AccelerationTag>;
using AccelerationScalar = NewType<float, struct AccelerationScalarTag>;
using Speed = NewType<float, struct SpeedTag>;
using Velocity = NewType<glm::vec3, struct VelocityTag>;
using VelocityDamping = NewType<float, struct VelocityDampingTag>;

using IsPointerInside = NewType<bool, struct IsPointerInsideTag>;
using IsPointerDownEventAccepted = NewType<
  bool,
  struct IsPointerDownEventAcceptedTag>;

struct Available
{};
struct Gravity
{};
struct Opaque
{};

struct ActorKind
{};
struct DirectionalLightKind
{};
struct EnemyKind
{};
struct EnemyPointer
{};
struct DebrisKind
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
struct GUIKind
{};

struct UIRect
{};

using ParticlesEmitter = std::function<void(glm::vec3 position)>;
using DebrisEmitter = NewType<
  std::function<void(std::shared_ptr<entt::registry>& registry,
                     const glm::vec3& position)>,
  struct DebrisEmitterTag>;

struct PointLightParams
{
  float constant { 1.0 };
  float linear { 1.0 };
  float quadratic { 1.0 };
};

struct Lives
{
  int max { 0 };
  int value { 0 };
};
