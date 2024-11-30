#include <filesystem>
#include <string>
#include <unordered_map>

std::unordered_map<std::string, std::string>
load_translations(const std::filesystem::path& file_path);
