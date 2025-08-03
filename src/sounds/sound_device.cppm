module;
#include <AL/al.h>
#include <AL/alc.h>
#include <cstring>

export module pln.sounds.sound_device;

import pln.services.logger;
import pln.sounds.utils;

namespace pln::sounds {

export
class SoundDevice {
public:
  SoundDevice(const SoundDevice&) = delete;
  SoundDevice(SoundDevice&&) = delete;
  SoundDevice operator=(const SoundDevice&) = delete;
  SoundDevice operator=(SoundDevice&&) = delete;

  SoundDevice()
  {
    print_audio_devices(alcGetString(nullptr, ALC_DEVICE_SPECIFIER));

    auto enumeration = alcIsExtensionPresent(nullptr, "ALC_ENUMERATION_EXT");
    if (enumeration == AL_TRUE) {
      pln::services::logger().info("OpenAL supports ALC_ENUMERATION_EXT");
    }

    // NOLINTNEXTLINE(cppcoreguidelines-prefer-member-initializer)
    _device = alcOpenDevice(nullptr);
    if (!_device) {
      pln::services::logger().error("Can't initialize OpenAL device");
      return;
    }
    pln::services::logger().info("Initialized OpenAL device");

    // NOLINTNEXTLINE(cppcoreguidelines-prefer-member-initializer)
    _context = alcCreateContext(_device, nullptr);
    if (!_context) {
      pln::services::logger().error("Can't initialize OpenAL context");
      alcCloseDevice(_device);
      return;
    }
    pln::services::logger().info("Initialized OpenAL context");

    if (!alcMakeContextCurrent(_context)) {
      pln::services::logger().error("Can't make OpenAL context current");
      alcDestroyContext(_context);
      alcCloseDevice(_device);
      return;
    }
  }

  ~SoundDevice()
  {
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(_context);
    alcCloseDevice(_device);
  }

private:
  ALCdevice *_device {nullptr};
  ALCcontext *_context {nullptr};

  static void print_audio_devices(const ALCchar* devices)
  {
    auto &log = pln::services::logger();
    auto current_device_name = devices;
    auto current_device_name_length = std::strlen(current_device_name);

    log.info("----- OpenAL devices -----");
    while (current_device_name_length > 0) {
      log.info(current_device_name);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
      current_device_name += current_device_name_length;
      current_device_name_length = std::strlen(current_device_name);
    }
    log.info("----- -----");
  }

};

}
