module;
#include <AL/al.h>
#include <AL/alc.h>
#include <format>
#include <source_location>
#include <string>

export module pln.sounds.utils;

import pln.services.logger;

namespace pln::sounds {

std::string
error_to_string(int error) {
  switch (error) {
  case AL_NO_ERROR: return {"No errors"};
  case AL_INVALID_NAME: return {"A bad name (ID) was passed to an OpenAL function"};
  case AL_INVALID_ENUM: return {"An invalid enum value was passed to an OpenAL function"};
  case AL_INVALID_VALUE: return {"An invalid value was passed to an OpenAL function"};
  case AL_INVALID_OPERATION: return {"The requested operation is not valid"};
  case AL_OUT_OF_MEMORY: return {"The requested operation resulted in OpenAL running out of memory"};
  default: return {"Unknown OpenAL error"};
  }
}


export
void
check_al_error(ALCdevice* device,
               const std::string& success_message,
               const std::string& error_message,
               std::source_location location = std::source_location::current())
{
    auto error = alcGetError(device);
    if (error == AL_NO_ERROR) {
      pln::services::logger().info(success_message);
    } else {
      pln::services::logger().error(error_message);
      pln::services::logger().error(std::format("{}:{} {}",
                                    location.file_name(),
                                    location.line(),
                                    error_to_string(error)));
    }
}


export
void
check_al_error(const std::string& success_message,
               const std::string& error_message,
               std::source_location location = std::source_location::current())
{
    auto error = alGetError();
    if (error == AL_NO_ERROR) {
      pln::services::logger().info(success_message);
    } else {
      pln::services::logger().error(error_message);
      pln::services::logger().error(std::format("{}:{} {}",
                                    location.file_name(),
                                    location.line(),
                                    error_to_string(error)));
    }
}


export
void
check_al_error(const std::string& error_message,
               std::source_location location = std::source_location::current())
{
    auto error = alGetError();
    if (error != AL_NO_ERROR) {
      pln::services::logger().error(error_message);
      pln::services::logger().error(std::format("{}:{} {}",
                                    location.file_name(),
                                    location.line(),
                                    error_to_string(error)));
    }
}

}
