#pragma once
#include <string>

#include "src/app.h"

void
load_level(const std::string& entities_file_path,
           const std::string& level_file_path,
           App& app);
