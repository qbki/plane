#include <SDL_mixer.h>
#include <utility>

#include "src/components/percent.h"
#include "src/events/event_emitter.h"
#include "src/fileio/settings_io.h"
#include "src/utils/system.h"

#include "settings.h"

Settings::Settings(std::filesystem::path settings_path)
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
Settings::master_volume()
{
  return _master_volume;
}

void
Settings::load()
{
  auto exec_path = get_excutable_path();
  const auto settings = load_settings(exec_path / _settings_path);
  _master_volume.value(settings.volume);
}

void
Settings::save() const
{
  auto exec_path = get_excutable_path();
  save_settings(exec_path / _settings_path,
                {
                  .volume = _master_volume.value(),
                });
}
