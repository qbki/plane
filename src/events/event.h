#pragma once
#include <entt/entt.hpp>
#include <filesystem>
#include <functional>
#include <glm/vec2.hpp>
#include <variant>

namespace Events {

struct GUILayout
{};

struct ShootEvent
{
  std::filesystem::path sound_path;
  float volume { 1.0 };
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

struct LoadNextLevelEvent
{};

struct LoadCurrentLevelEvent
{};

struct LoseEvent
{};

template<typename T>
using CB = std::function<void(const T&)>;

using Event = std::variant<ShootEvent>;

}
