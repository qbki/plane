#pragma once
#include <algorithm>
#include <entt/core/type_traits.hpp>
#include <entt/entity/fwd.hpp>
#include <functional>
#include <glm/detail/qualifier.hpp>
#include <glm/vec3.hpp>
#include <iterator>
#include <memory>
#include <utility>
#include <vector>

#include "game_state/texture_type.h"
#include "mesh.h"
#include "texture.h"
#include "utils/index.h"

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
using Position = NewType<glm::vec3, struct PositionTag>;
using Range = NewType<float, struct RangeTag>;
using Rotation = NewType<glm::vec3, struct RotationTag>;
using Speed = NewType<float, struct SpeedTag>;
using TexturePointer = std::shared_ptr<Texture>;

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

using ParticlesEmitter = std::function<void(glm::vec3 position)>;
using ProjectileEmitter = std::function<void()>;

class Textures
{
private:
  using TextureList = std::shared_ptr<std::vector<Texture>>;
  TextureList _textures;
  size_t _current_texture_idx = 0;

public:
  explicit Textures(TextureList textures)
    : _textures(std::move(textures))
  {
    change_type(TextureType::Type::MAIN);
  }

  void change_type(TextureType::Type type)
  {
    auto found_it = std::ranges::find_if(*_textures, [&](Texture& value) {
      return value.type() == static_cast<unsigned int>(type);
    });
    if (found_it != end(*_textures)) {
      _current_texture_idx = std::distance(begin(*_textures), found_it);
    }
  }

  [[nodiscard]] Texture& texture() const
  {
    return (*_textures)[_current_texture_idx];
  }
};

struct Velocity
{
  float scalar_acceleration;
  float damping;
  glm::vec3 acceleration;
  glm::vec3 velocity;
  Velocity()
    : scalar_acceleration(0)
    , damping(0)
    , acceleration(zero<glm::vec3>())
    , velocity(zero<glm::vec3>())
  {
  }
  Velocity(float _acceleration, float _damping)
    : scalar_acceleration(_acceleration)
    , damping(_damping)
    , acceleration(glm::vec3(0.0))
    , velocity(glm::vec3(0.0))
  {
  }
  Velocity(glm::vec3 _initial_velocity, float _damping)
    : scalar_acceleration(0)
    , damping(_damping)
    , acceleration(glm::vec3(0.0))
    , velocity(_initial_velocity)
  {
  }
};

struct PointLightParams
{
  float constant = 1.0;
  float linear = 1.0;
  float quadratic = 1.0;
};
