#pragma once
#include <memory>

#include "src/gui/font.h"
#include "src/utils/types.h"

namespace GUI {

struct Typography
{
  constexpr static const unsigned int SIZE_BODY_1{ 16 };
  constexpr static const Core::Color DEFAULT_FONT_COLOR{ 0xff'ff'ff'ff };

  std::shared_ptr<Font> body_1;
};

struct Theme
{
  Typography typography;
};

}
