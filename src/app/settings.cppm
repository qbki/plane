module;
#include <SDL_mixer.h>
#include <filesystem>
#include <utility>

export module pln.app.settings;

import pln.components.percent;
import pln.fileio.settings_io;
import pln.utils.system;

using namespace pln::components;
using namespace pln::fileio;

namespace pln::app {

export
class Settings
{
private:
  std::filesystem::path _settings_path;
  Percent _master_volume;

public:
  Settings(std::filesystem::path settings_path)
    : _settings_path(std::move(settings_path))
    , _master_volume({})
  {
    _master_volume.value(Percent::TOP);
    _master_volume.on_changed().add([this](auto) {
      auto volume = this->master_volume().norm() * MIX_MAX_VOLUME;
      auto mix_volume = static_cast<unsigned char>(volume);
      Mix_MasterVolume(mix_volume);
    });
  }


  Percent&
  master_volume()
  {
    return _master_volume;
  }


  void
  load()
  {
    auto exec_path = pln::utils::system::get_excutable_path();
    const auto settings = load_settings(exec_path / _settings_path);
    _master_volume.value(settings.volume);
  }


  void
  save() const
  {
    auto exec_path = pln::utils::system::get_excutable_path();
    save_settings(exec_path / _settings_path,
                  {
                    .volume = _master_volume.value(),
                  });
  }
};

}
