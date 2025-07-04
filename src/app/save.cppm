module;
#include <utility>
#include <filesystem>

export module pln.app.save;

import pln.fileio.save_data_io;
import pln.utils.system;

using namespace pln::fileio;

namespace pln::app {

export class Save
{
private:
  std::filesystem::path _save_data_path;

public:
  Save(std::filesystem::path save_data_path)
    : _save_data_path(std::move(save_data_path))
  {
  }


  [[nodiscard]] SaveData
  load(const std::filesystem::path& assets_dir_path) const
  {
    auto exec_path = pln::utils::system::get_excutable_path();
    return load_save_data(exec_path / _save_data_path, assets_dir_path);
  }


  void
  save(const std::filesystem::path& assets_dir_path, const SaveData& value) const
  {
    auto exec_path = pln::utils::system::get_excutable_path();
    store_save_data(exec_path / _save_data_path, assets_dir_path, value);
  }
};

}
