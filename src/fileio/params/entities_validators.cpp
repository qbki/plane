#include <string>

#include "src/utils/crash.h"

#include "entities_validators.h"

void
validate(const EntityParamsText& value)
{
  if (value.size <= 0) {
    crash(R"("size" field of kind "text" must be 1 or more)");
  }

  if (value.width < 0.0f) {
    crash(R"("width" field of kind "text" must be more than zero)");
  }

  if (value.height < 0.0f) {
    crash(R"("height" field of kind "text" must be more than zero)");
  }
}
