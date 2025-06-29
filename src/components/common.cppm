module;
#include <entt/entity/fwd.hpp>
#include <functional> // IWYU pragma: export
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <optional> // IWYU pragma: export
#include <vector>

export module pln.components.common;

import pln.control;
import pln.mesh;
import pln.textures;
import pln.utils.types;

using namespace pln::utils;

namespace pln::components {

export
enum class EnemyStateEnum
{
  HUNTING = 0,
  INACTIVE,
};

export using Children = NewType<std::vector<entt::entity>, struct ChildrenTag>;
export using Color = NewType<glm::vec3, struct ColorTag>;
export using Direction = NewType<glm::vec3, struct DirectionTag>;
export using InitialPosition = NewType<glm::vec3, struct InitialPositionTag>;
export using IsDirty = NewType<bool, struct IsDirtyTag>;
export using Lifetime = NewType<float, struct LifeTimeTag>;
export using LifetimeMax = NewType<float, struct LifeTimeMaxTag>;
export using MeshPointer = std::shared_ptr<pln::mesh::Mesh>;
export using Owner = NewType<entt::entity, struct OwnerTag>;
export using Parent = NewType<std::optional<entt::entity>, struct ParentTag>;
export using ShotSound = NewType<std::optional<std::string>, struct ShotSoundTag>;
export using Text = NewType<std::string, struct TextTag>;
export using TexturePointer = std::shared_ptr<pln::textures::Texture>;
export using TutorialButton = NewType<pln::control::Action, struct TutorialButtonTag>;

export using Acceleration = NewType<glm::vec3, struct AccelerationTag>;
export using AccelerationScalar = NewType<float, struct AccelerationScalarTag>;
export using Speed = NewType<float, struct SpeedTag>;
export using Velocity = NewType<glm::vec3, struct VelocityTag>;
export using VelocityDamping = NewType<float, struct VelocityDampingTag>;

export using IsPointerInside = NewType<bool, struct IsPointerInsideTag>;
export using IsPointerDownEventAccepted = NewType<
  bool,
  struct IsPointerDownEventAcceptedTag>;

export
struct Available
{};

export
struct Gravity
{};

export
struct Opaque
{};

export
struct ActorKind
{};

export
struct DirectionalLightKind
{};

export
struct EnemyKind
{};

export
struct EnemyPointer
{};

export
struct DebrisKind
{};

export
struct ParticleKind
{};

export
struct PlayerKind
{};

export
struct PointLightKind
{};

export
struct ProjectileKind
{};

export
struct TutorialButtonKind
{};

export
struct GUIKind
{};

export
struct UIRect
{};

export using ParticlesEmitter = std::function<void(glm::vec3 position)>;
export using DebrisEmitter = NewType<
  std::function<void(std::shared_ptr<entt::registry>& registry,
                                     const glm::vec3& position)>,
                struct DebrisEmitterTag>;

export
struct PointLightParams
{
  float constant { 1.0 };
  float linear { 1.0 };
  float quadratic { 1.0 };
};

export
struct Lives
{
  int max { 0 };
  int value { 0 };
};

}
