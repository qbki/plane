#include <SDL_mixer.h>

#include "sound.h"

namespace Sound {

Sound::Sound(Mix_Chunk* chunk)
  : _chunk(chunk) {};

Sound::~Sound()
{
  Mix_FreeChunk(_chunk);
}

void
Sound::volume(float value)
{
  Mix_VolumeChunk(_chunk, volume_to_integer(value));
}

float
Sound::volume()
{
  auto volume = Mix_VolumeChunk(_chunk, -1);
  return static_cast<float>(volume) / static_cast<float>(MIX_MAX_VOLUME);
}

/**
 * By default this function sets the max volume for a channel.
 */
void
Sound::play(float volume)
{
  auto channel = Mix_PlayChannel(FIRST_FREE_CHANNEL, _chunk, 0);
  if (channel >= 0) {
    Mix_Volume(channel, volume_to_integer(volume));
  }
}

/**
 * Expected to be used in Mix_Volume or Mix_VolumeChunk
 * which clamp the volume value.
 */
int
Sound::volume_to_integer(float volume)
{
  return static_cast<int>(volume * MIX_MAX_VOLUME);
}

}
