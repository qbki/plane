module;
#include <filesystem>
#include <glm/vec3.hpp>
#include <optional>

export module pln.app.info;

import pln.components.common;

namespace pln::app {

export
struct Info
{
  std::optional<std::filesystem::path> current_level { std::nullopt };
  int hostiles { 0 };
  pln::components::Lives lives {};
  glm::vec3 player_position { 0, 0, 0 };
};

}
