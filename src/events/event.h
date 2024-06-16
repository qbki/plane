#pragma once
#include <entt/entity/fwd.hpp>
#include <functional>
#include <glm/vec2.hpp>
#include <string_view>
#include <variant>

namespace Events {

struct GUILayout
{};

struct ShootEvent
{
  std::string_view sound_path;
};

struct PointerEnter
{};

struct PointerLeave
{};

struct PointerMove
{
  glm::ivec2 position;
};

struct PointerDown
{
  glm::ivec2 position;
};

struct LoadLevelEvent
{};

template<typename T>
using CB = std::function<void(const T&)>;

using Event = std::variant<ShootEvent>;

}
