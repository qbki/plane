module;
#include <AL/al.h>
#include <AL/alc.h>
#include <SDL2/SDL_audio.h>
#include <filesystem>
#include <format>

export module pln.sounds.sound_buffer;

import pln.services.logger;
import pln.sounds.utils;
import pln.utils.crash;

namespace pln::sounds {

export
class SoundBuffer {
public:
  SoundBuffer(const SoundBuffer&) = delete;
  SoundBuffer(SoundBuffer&&) = delete;
  SoundBuffer operator=(const SoundBuffer&) = delete;
  SoundBuffer operator=(SoundBuffer&&) = delete;


  SoundBuffer(const std::filesystem::path &path)
  {
    uint32_t data_size {0};

    SDL_AudioSpec audio_spec;
    auto *result = SDL_LoadWAV(path.c_str(),
                               &audio_spec,
                               &_data_buffer,
                               &data_size);
    if (!result) {
      utils::crash(std::format("Can't load WAV file: {}", path.c_str()));
    }
    alGenBuffers(static_cast<ALuint>(1), &_al_buffer);
    check_al_error("Can't create OpenAL buffer");

    auto format {get_format(audio_spec.channels, audio_spec.format)};
    alBufferData(_al_buffer,
                 format,
                 _data_buffer,
                 static_cast<int>(data_size),
                 audio_spec.freq);
    check_al_error("Can't setup OpenAL buffer");
  }


  ~SoundBuffer()
  {
    auto *context = alcGetCurrentContext();
    if (context) {
      alDeleteBuffers(static_cast<ALuint>(1), &_al_buffer);
      check_al_error("Can't remove OpenAL buffer");
    }
    SDL_FreeWAV(_data_buffer);
  }


  ALuint buffer_idx()
  {
    return _al_buffer;
  }

private:
  uint8_t *_data_buffer {nullptr};
  ALuint _al_buffer {0};


  int get_format(int channels, int samples)
  {
    bool is_stereo {channels > 1};

    switch (samples) {
    case AUDIO_U8:
    case AUDIO_S8:
      return is_stereo
        ? AL_FORMAT_STEREO8
        : AL_FORMAT_MONO8;
    case AUDIO_S16:
    case AUDIO_U16:
      return is_stereo
        ? AL_FORMAT_STEREO16
        : AL_FORMAT_MONO16;
    default:
      return -1;
    }
  }
};

}
