#pragma once
#include <memory>

#include "src/gui/font.h"
#include "src/utils/types.h"

namespace GUI {

struct Typography
{
  constexpr static const unsigned int SIZE_H1{ 96 };
  constexpr static const unsigned int SIZE_H2{ 60 };
  constexpr static const unsigned int SIZE_H3{ 48 };
  constexpr static const unsigned int SIZE_H4{ 34 };
  constexpr static const unsigned int SIZE_H5{ 24 };
  constexpr static const unsigned int SIZE_H6{ 20 };
  constexpr static const unsigned int SIZE_BODY_1{ 16 };
  constexpr static const unsigned int SIZE_BODY_2{ 14 };

  constexpr static const Core::Color DEFAULT_FONT_COLOR{ 0xff'ff'ff'ff };

  std::shared_ptr<Font> h1;
  std::shared_ptr<Font> h2;
  std::shared_ptr<Font> h3;
  std::shared_ptr<Font> h4;
  std::shared_ptr<Font> h5;
  std::shared_ptr<Font> h6;
  std::shared_ptr<Font> body1;
  std::shared_ptr<Font> body2;
};

struct Theme
{
  Typography typography;
};

}
