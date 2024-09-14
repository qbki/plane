#include <SDL_mixer.h>

#include "sound.h"

namespace Sound {

Sound::Sound(Mix_Chunk* chunk)
  : _chunk(chunk){};

Sound::~Sound()
{
  Mix_FreeChunk(_chunk);
}

void
Sound::volume(float value)
{
  Mix_VolumeChunk(_chunk, static_cast<int>(value * MIX_MAX_VOLUME));
}

float
Sound::volume()
{
  auto volume = Mix_VolumeChunk(_chunk, -1);
  return static_cast<float>(volume) / static_cast<float>(MIX_MAX_VOLUME);
}

void
Sound::play()
{
  Mix_PlayChannel(FIRST_FREE_CHANNEL, _chunk, 0);
}

}
