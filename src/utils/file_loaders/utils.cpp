#include <filesystem>
#include <format>
#include <istream>
#include <stdexcept>
#include <string>

void
throw_error_if_file_not_found(const std::string& file_name)
{
  if (!std::filesystem::exists(file_name)) {
    throw std::runtime_error(std::format("File not found: {}", file_name));
  }
}

void
throw_error_if_file_is_not_readable(const std::istream& stream,
                                    const std::string& file_name)
{
  if (stream.fail()) {
    throw std::runtime_error(std::format("Cannot read a file: {}", file_name));
  }
}
