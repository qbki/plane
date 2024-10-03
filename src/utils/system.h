#pragma once
#include <filesystem>
#include <optional>

std::optional<std::filesystem::path>
get_excutable_path();
