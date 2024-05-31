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
Sound::play()
{
  Mix_PlayChannel(FIRST_FREE_CHANNEL, _chunk, 0);
}
}
