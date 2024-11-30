#include <stdexcept>

#include "entities_validators.h"

void
validate(const EntityParamsText& value)
{
  if (value.size <= 0) {
    throw std::runtime_error(
      R"("size" field of kind "text" must be 1 or more)");
  }
}
