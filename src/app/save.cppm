module;
#include <utility>
#include <filesystem>

#include "src/fileio/save_data_io.h"

export module pln.app.save;

import pln.utils.system;

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
  load() const
  {
    auto exec_path = pln::utils::system::get_excutable_path();
    return load_save_data(exec_path / _save_data_path);
  }


  void
  save(const SaveData& value) const
  {
    auto exec_path = pln::utils::system::get_excutable_path();
    store_save_data(exec_path / _save_data_path, value);
  }
};

}
