#pragma once
#include <SDL_mixer.h>

namespace Sound {

const int FIRST_FREE_CHANNEL = -1;

class Sound
{
private:
  Mix_Chunk* _chunk;

public:
  explicit Sound(Mix_Chunk* chunk);
  Sound(const Sound&) = delete;
  Sound(Sound&&) = delete;
  Sound& operator=(const Sound&) = delete;
  Sound& operator=(Sound&&) = delete;
  ~Sound();

  void play();
};

}
