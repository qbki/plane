module;
#include <AL/al.h>
#include <AL/alc.h>

export module pln.sounds.sound_source;

import pln.services.logger;
import pln.sounds.utils;

namespace pln::sounds {

export
class SoundSource {
public:
  SoundSource(const SoundSource&) = delete;
  SoundSource &operator=(const SoundSource&) = delete;

  SoundSource(SoundSource&& other)
    : _source(other._source)
  {
    other._source = 0;
  };

  SoundSource &operator=(SoundSource&& other)
  {
    _source = other._source;
    other._source = 0;
    return *this;
  }

  explicit SoundSource(ALuint buffer)
  {
    alGenSources(static_cast<ALuint>(1), &_source);
    check_al_error("Can't create an OpenAL source");

    alSourcef(_source, AL_PITCH, 1);
    check_al_error("Can't setup OpenAL pitch");

    alSourcef(_source, AL_GAIN, 1);
    check_al_error("Can't setup OpenAL gain");

    alSource3f(_source, AL_POSITION, 0, 0, 0);
    check_al_error("Can't setup OpenAL position");

    alSource3f(_source, AL_VELOCITY, 0, 0, 0);
    check_al_error("Can't setup OpenAL velocity");

    alSourcei(_source, AL_LOOPING, AL_FALSE);
    check_al_error("Can't setup OpenAL looping");

    alSourcei(_source, AL_BUFFER, static_cast<ALint>(buffer));
    check_al_error("Can't apply OpenAL buffer");
  }

  ~SoundSource()
  {
    if (_source > 0) {
      alDeleteSources(static_cast<ALuint>(1), &_source);
      check_al_error("Can't delete OpenAL source");
    }
  }

  void play()
  {
    alSourcePlay(_source);
    check_al_error("Can't play OpenAL source");
  }

private:
  ALuint _source {0};

};

}
