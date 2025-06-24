module;
#include <filesystem>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>

export module pln.fileio.save_data_io;

import pln.utils.file_loaders;
import pln.utils.system;

using namespace pln::utils::file_loaders;

namespace pln::fileio {

export
struct SaveData
{
  std::optional<std::filesystem::path> current_level = std::nullopt;
};


export
SaveData
load_save_data(const std::filesystem::path& save_path,
               const std::filesystem::path& assets_dir_path)
{
  auto exec_path = pln::utils::system::get_excutable_path();
  auto json = load_local_json(save_path).or_fallback({});
  auto current_level = json.contains("current_level")
                         ? json["current_level"].get<std::string>()
                         : "";
  return { .current_level = current_level.empty()
                              ? std::nullopt
                              : std::make_optional(assets_dir_path
                                                   / current_level) };
}


export
void
store_save_data(const std::filesystem::path& load_path,
                const std::filesystem::path& assets_dir_path,
                const SaveData& save_data)
{
  using namespace std::filesystem;
  auto current_level = save_data.current_level.value().empty()
                         ? ""
                         : relative(save_data.current_level.value(),
                                    assets_dir_path);
  nlohmann::json json = { { "current_level", current_level } };
  save_local_json(load_path, json);
}

}
