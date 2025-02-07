#ifdef __linux__
#include <array>
#include <linux/limits.h> // IWYU pragma: keep
#include <unistd.h>

#include "src/utils/crash.h"

#include "system.h"

std::filesystem::path
get_excutable_path()
{
  std::array<char, PATH_MAX> path {};
  auto size = readlink("/proc/self/exe", path.data(), PATH_MAX);
  if (size > 0) {
    std::filesystem::path result { path.data() };
    return { result.parent_path() };
  }
  crash("Can't get the executable path");
}

#elif __EMSCRIPTEN__

#include "system.h"

std::filesystem::path
get_excutable_path()
{
  return { "/" };
}

#endif
