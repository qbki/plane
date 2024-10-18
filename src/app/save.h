#pragma once
#include <filesystem>

#include "src/fileio/save_data_io.h"

class Save
{
private:
  std::filesystem::path _save_data_path;

public:
  explicit Save(std::filesystem::path save_data_path);

  [[nodiscard]] SaveData load() const;
  void save(const SaveData& value) const;
};
