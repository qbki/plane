#pragma once
#include "src/logger/abstract_logger.h" // IWYU pragma: export

import pln;

namespace Services {

inline AbstractLogger&
logger()
{
  return pln::Service<AbstractLogger>::get();
}

}
