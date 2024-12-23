#include <stdexcept>

#include "entities_validators.h"

void
validate(const EntityParamsText& value)
{
  if (value.size <= 0) {
    throw std::runtime_error(
      R"("size" field of kind "text" must be 1 or more)");
  }

  if (value.width.has_value() && (value.width.value() < 0.0f)) {
    throw std::runtime_error(
      R"("width" field of kind "text" must be more than zero)");
  }

  if (value.height.has_value() && (value.height.value() < 0.0f)) {
    throw std::runtime_error(
      R"("height" field of kind "text" must be more than zero)");
  }
}
