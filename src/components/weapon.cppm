module;
#include <filesystem> // IWYU pragma: export
#include <limits>
#include <optional>
#include <string>

export module pln.components.weapon;

namespace pln::components {

export
struct Weapon
{
  float bullet_speed { 0.0 };
  float spread { 0.0 };    // radians
  float lifetime { 0.0 };  // seconds
  float fire_rate { 0.0 }; // projectile/second
  std::optional<std::filesystem::path> shot_sound_path {};
  std::string bullet_model_path { "" };
  bool is_shooting { false };
  float left_time_to_shoot { std::numeric_limits<float>().min() }; // seconds
};

}
