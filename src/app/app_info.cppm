module;
#include <filesystem>
#include <optional>

#include "src/components/common.h"

export module pln.app.info;

namespace pln::app {

export
struct Info
{
  std::optional<std::filesystem::path> current_level { std::nullopt };
  int hostiles { 0 };
  Lives lives {};
  glm::vec3 player_position { 0, 0, 0 };
};

}
