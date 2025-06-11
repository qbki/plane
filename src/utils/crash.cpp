#include <cstdlib>

#include "src/services/logger.h"

#include "crash.h"

[[noreturn]] void
crash(const std::string& message)
{
  Services::logger().error(message);
  std::abort();
}

[[noreturn]] void
crash(const std::exception& error)
{
  crash(error.what());
}
