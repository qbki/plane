#pragma once
#include "logger/abstract_logger.h" // IWYU pragma: export

#include "service.h"

inline AbstractLogger&
logger()
{
  return Service<AbstractLogger>::get();
}
