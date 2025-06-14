#include <utility>

#include "src/fileio/save_data_io.h"

import utils.system;

#include "save.h"

Save::Save(std::filesystem::path save_data_path)
  : _save_data_path(std::move(save_data_path))
{
}

SaveData
Save::load() const
{
  auto exec_path = pln::utils::system::get_excutable_path();
  return load_save_data(exec_path / _save_data_path);
}

void
Save::save(const SaveData& value) const
{
  auto exec_path = pln::utils::system::get_excutable_path();
  store_save_data(exec_path / _save_data_path, value);
}
