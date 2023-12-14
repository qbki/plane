#include "service.h"
#include "logger/index.h"


inline AbstractLogger& logger() {
  return Service<AbstractLogger>::get();
}
