#pragma once
#include <filesystem>

#include "src/components/percent.h"

class Settings
{
private:
  std::filesystem::path _settings_path;
  Percent _master_volume;

public:
  explicit Settings(std::filesystem::path settings_path);

  Percent& master_volume();
  void load();
  void save() const;
};
