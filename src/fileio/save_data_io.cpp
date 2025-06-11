#include <filesystem>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>

#include "src/fileio/save_data_io.h"
#include "src/services.h"
#include "src/utils/file_loaders.h"
#include "src/utils/system.h"

#include "save_data_io.h"

SaveData
load_save_data(const std::filesystem::path& path)
{
  auto exec_path = get_excutable_path();
  auto json = load_local_json(path).or_fallback({});
  auto current_level = json.contains("current_level")
                         ? json["current_level"].get<std::string>()
                         : "";
  auto assets_dir = Services::app().assets_dir();
  return { .current_level = current_level.empty()
                              ? std::nullopt
                              : std::make_optional(assets_dir
                                                   / current_level) };
}

void
store_save_data(const std::filesystem::path& path, const SaveData& save_data)
{
  using namespace std::filesystem;
  auto current_level = save_data.current_level.value().empty()
                         ? ""
                         : relative(save_data.current_level.value(),
                                    Services::app().assets_dir());
  nlohmann::json json = { { "current_level", current_level } };
  save_local_json(path, json);
}
