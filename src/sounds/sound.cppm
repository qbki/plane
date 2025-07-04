module;
#include <SDL_mixer.h>

export module pln.sounds.sound;

namespace pln::sounds {

constexpr int FIRST_FREE_CHANNEL = -1;

export
class Sound
{
private:
  Mix_Chunk* _chunk;

public:
  Sound(const Sound&) = delete;
  Sound(Sound&&) = delete;
  Sound& operator=(const Sound&) = delete;
  Sound& operator=(Sound&&) = delete;

  Sound(Mix_Chunk* chunk)
    : _chunk(chunk) {};


  ~Sound()
  {
    Mix_FreeChunk(_chunk);
  }


  void
  volume(float value)
  {
    Mix_VolumeChunk(_chunk, volume_to_integer(value));
  }


  float
  volume()
  {
    auto volume = Mix_VolumeChunk(_chunk, -1);
    return static_cast<float>(volume) / static_cast<float>(MIX_MAX_VOLUME);
  }


  /**
   * By default this function sets the max volume for a channel.
   */
  void
  play(float volume)
  {
    auto channel = Mix_PlayChannel(FIRST_FREE_CHANNEL, _chunk, 0);
    if (channel >= 0) {
      Mix_Volume(channel, volume_to_integer(volume));
    }
  }


private:
  /**
   * Expected to be used in Mix_Volume or Mix_VolumeChunk
   * which clamp the volume value.
   */
  int
  static volume_to_integer(float volume)
  {
    return static_cast<int>(volume * MIX_MAX_VOLUME);
  }
};

}
