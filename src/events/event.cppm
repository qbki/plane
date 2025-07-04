module;
#include <entt/entt.hpp>
#include <filesystem>
#include <functional>
#include <glm/vec2.hpp>
#include <variant>

export module pln.events.event;

namespace pln::events {

export
struct GUILayout
{};

export
struct ShootEvent
{
  std::filesystem::path sound_path;
  float volume { 1.0 };
};

export
struct PointerEnter
{};

export
struct PointerLeave
{};

export
struct PointerMove
{
  glm::ivec2 position;
};

export
struct PointerDown
{
  glm::ivec2 position;
};

export
struct LoadNextLevelEvent
{};

export
struct LoadCurrentLevelEvent
{};

export
struct LoseEvent
{};

export
template<typename T>
using CB = std::function<void(const T&)>;

export
using Event = std::variant<ShootEvent>;

}
