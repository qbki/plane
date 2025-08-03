module;
#include <AL/al.h>
#include <array>
#include <glm/vec3.hpp>

export module pln.sounds.sound_listener;

import pln.sounds.utils;

namespace pln::sounds {

export
void set_listener(glm::vec3 position,
                  glm::vec3 velocity,
                  glm::vec3 forward,
                  glm::vec3 up)
{
  constexpr int SIZE {6};
  std::array<ALfloat, SIZE> orientation { forward.x, forward.y, forward.z, //
                                          up.x, up.y, up.z };
  alListener3f(AL_POSITION, position.x, position.y, position.z);
  check_al_error("Can't setup OpenAL listener position");

  alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
  check_al_error("Can't setup OpenAL listener velocity");

  alListenerfv(AL_ORIENTATION, orientation.data());
  check_al_error("Can't setup OpenAL listener orientation");
};

}
