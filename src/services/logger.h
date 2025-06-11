#pragma once
#include "src/logger/abstract_logger.h" // IWYU pragma: export

#include "src/service.h"

namespace Services {

inline AbstractLogger&
logger()
{
  return Service<AbstractLogger>::get();
}

}
