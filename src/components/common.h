#pragma once
#include <entt/entity/fwd.hpp>
#include <functional> // IWYU pragma: export
#include <glm/gtc/constants.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <optional> // IWYU pragma: export
#include <vector>

#include "src/control.h"
#include "src/mesh.h" // IWYU pragma: export
#include "src/texture.h"
#include "src/utils/types.h" // IWYU pragma: export

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
using Lives = NewType<int, struct LivesTag>;
using MeshPointer = std::shared_ptr<Mesh>;
using Owner = NewType<entt::entity, struct OwnerTag>;
using Parent = NewType<std::optional<entt::entity>, struct ParentTag>;
using ShotSound = NewType<std::optional<std::string>, struct ShotSoundTag>;
using Speed = NewType<float, struct SpeedTag>;
using Text = NewType<std::string, struct TextTag>;
using TexturePointer = std::shared_ptr<Texture>;
using TutorialButton = NewType<Control::Action, struct TutorialButtonTag>;
using UniqueTexturePtr = std::unique_ptr<Texture>;

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

struct DirectionalLightKind
{};
struct EnemyKind
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

using ParticlesEmitter = std::function<void(glm::vec3 position)>;
using DebrisEmitter = NewType<
  std::function<void(entt::registry& registry, const glm::vec3& position)>,
  struct DebrisEmitterTag>;

struct PointLightParams
{
  float constant = 1.0;
  float linear = 1.0;
  float quadratic = 1.0;
};
