#include <filesystem>
#include <nlohmann/json.hpp>
#include <optional>
#include <stdexcept>
#include <string>

#include "src/consts.h"
#include "src/fileio/save_data_io.h"
#include "src/services.h"
#include "src/utils/file_loaders.h"
#include "src/utils/system.h"

#include "save_data_io.h"

SaveData
load_save_data(const std::filesystem::path& path)
{
  try {
    auto exec_path = get_excutable_path();
    auto json = load_local_json(path);
    auto current_level = json.value<std::string>("current_level", "");
    return { .current_level = current_level.empty()
                                ? std::nullopt
                                : std::make_optional(exec_path / ASSETS_DIR
                                                     / current_level) };
  } catch (const std::runtime_error& error) {
    Services::logger().warn("Can't load a safe file, used default values.");
    Services::logger().warn(error.what());
  }
  return {};
}

void
store_save_data(const std::filesystem::path& path, const SaveData& save_data)
{
  using namespace std::filesystem;
  auto exec_path = get_excutable_path();
  auto current_level = relative(save_data.current_level.value(),
                                exec_path / ASSETS_DIR);
  nlohmann::json json = { { "current_level", current_level } };
  save_local_json(path, json);
}
