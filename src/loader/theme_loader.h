#include <filesystem>
#include <memory>

#include "src/gui/theme.h"

std::unique_ptr<GUI::Theme>
load_theme(const std::filesystem::path& theme_file_path);
