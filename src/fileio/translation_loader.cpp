#include <format>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <unordered_map>

#include "src/services.h"
#include "src/utils/file_loaders/file_loaders.h"

#include "translation_loader.h"

std::unordered_map<std::string, std::string>
load_translations(const std::filesystem::path& file_path)
{
  nlohmann::json json;
  try {
    json = load_json(file_path);
  } catch (const std::runtime_error& error) {
    Services::logger().error(
      std::format("Can't load translations: {}", error.what()));
    return {};
  }

  if (!json.is_object()) {
    auto message = std::format("Root element of \"{}\" must be an object",
                               file_path.string());
    throw std::runtime_error(message);
  }

  std::unordered_map<std::string, std::string> translations;
  for (auto& [key, value] : json.items()) {
    if (!value.is_string()) {
      constexpr auto templ = R"(Value of key "{}" in "{}" must be a string)";
      auto message = std::format(templ, key, file_path.string());
      throw std::runtime_error(message);
    }
    translations[key] = value;
  }

  return translations;
}
