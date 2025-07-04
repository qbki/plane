module;
#include <exception>
#include <string>

export module pln.utils.crash;

import pln.services.logger;

namespace pln::utils {


export
[[noreturn]] void
crash(const std::string& message)
{
  pln::services::logger().error(message);
  std::abort();
}


export
[[noreturn]] void
crash(const std::exception& error)
{
  crash(error.what());
}


}
