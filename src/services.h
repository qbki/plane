#include "logger/index.h"
#include "service.h"

inline AbstractLogger&
logger()
{
  return Service<AbstractLogger>::get();
}
