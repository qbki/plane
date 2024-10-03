#ifdef __linux__
#include <array>
#include <linux/limits.h>
#include <unistd.h>

#include "system.h"

std::optional<std::filesystem::path>
get_excutable_path() {
  std::array<char, PATH_MAX> path {};
  auto size = readlink("/proc/self/exe", path.data(), PATH_MAX);
  if (size > 0) {
    std::filesystem::path result {path.data()};
    return {result.parent_path()};
  }
  return std::nullopt;
}

#elif __EMSCRIPTEN__

#include "system.h"

std::optional<std::filesystem::path>
get_excutable_path() {
  return {"/"};
}

#endif
