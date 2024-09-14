#include <SDL_mixer.h>

#include "src/components/percent.h"
#include "src/events/event_emitter.h"

#include "settings.h"

Settings::Settings()
  : _master_volume({})
{
  _master_volume.value(1.0);
  _master_volume.on_changed().add([](double volume) {
    auto mix_volume = static_cast<unsigned char>(volume * MIX_MAX_VOLUME);
    Mix_MasterVolume(mix_volume);
  });
}

Percent&
Settings::master_volume()
{
  return _master_volume;
}
