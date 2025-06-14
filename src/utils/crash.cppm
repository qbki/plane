module;
#include <exception>
#include <string>

#include "src/services/logger.h"

export module utils.crash;

namespace pln::utils {


export
[[noreturn]] void
crash(const std::string& message)
{
  Services::logger().error(message);
  std::abort();
}


export
[[noreturn]] void
crash(const std::exception& error)
{
  crash(error.what());
}


}
