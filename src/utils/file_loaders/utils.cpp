#include <filesystem>
#include <format>
#include <memory>
#include <stdexcept>
#include <string>

#include "src/utils/result.h"

Result<const std::string>
does_file_exist(const std::string& file_path)
{
  static auto make_error = [](const std::string& path) {
    auto message = std::format("File not found: {}", path);
    return std::make_unique<std::runtime_error>("");
  };
  return { std::filesystem::exists(file_path)
             ? Result<const std::string>::from_payload(file_path)
             : Result<const std::string>::from_error(make_error(file_path)) };
}
