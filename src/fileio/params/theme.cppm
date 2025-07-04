module;
#include <filesystem>

export module pln.fileio.params.theme;

namespace pln::fileio::params {

export
struct ThemeParams
{
  std::filesystem::path font_source {};
  std::string font_family {};
};

}
