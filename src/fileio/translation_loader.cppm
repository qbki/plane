module;
#include <format>
#include <nlohmann/json.hpp>
#include <unordered_map>

export module pln.fileio.translation_loader;

import pln.services.logger;
import pln.utils.crash;
import pln.utils.file_loaders;

using namespace pln::utils::file_loaders;

namespace pln::fileio {

export
std::unordered_map<std::string, std::string>
load_translations(const std::filesystem::path& file_path)
{
  auto json_result = load_json(file_path);
  if (!json_result.has_payload()) {
    pln::services::logger().error(
      std::format("Can't load translations: {}", json_result.error()->what()));
    return {};
  }
  auto json = json_result.payload();

  if (!json.is_object()) {
    auto message = std::format("Root element of \"{}\" must be an object",
                               file_path.string());
    pln::utils::crash(message);
  }

  std::unordered_map<std::string, std::string> translations;
  for (auto& [key, value] : json.items()) {
    if (!value.is_string()) {
      constexpr auto templ = R"(Value of key "{}" in "{}" must be a string)";
      auto message = std::format(templ, key, file_path.string());
      pln::utils::crash(message);
    }
    translations[key] = value;
  }

  return translations;
}

}
