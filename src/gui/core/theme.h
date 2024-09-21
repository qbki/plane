#pragma once
#include <memory>

#include "src/gui/core/font.h"
#include "src/utils/types.h"

namespace GUI {

struct Palette
{
  constexpr static const Core::Color WHITE { 0x7f'7d'7e'ff };
  constexpr static const Core::Color WHITE_ACCENT { 0xff'fd'fe'ff };
  constexpr static const Core::Color GRAY { 0x43'43'43'ff };

  Core::Color white { WHITE };
  Core::Color white_accent { WHITE_ACCENT };
  Core::Color gray { GRAY };
};

struct Typography
{
  constexpr static const unsigned int SIZE_H1 { 96 };
  constexpr static const unsigned int SIZE_H2 { 60 };
  constexpr static const unsigned int SIZE_H3 { 48 };
  constexpr static const unsigned int SIZE_H4 { 34 };
  constexpr static const unsigned int SIZE_H5 { 24 };
  constexpr static const unsigned int SIZE_H6 { 20 };
  constexpr static const unsigned int SIZE_BODY_1 { 16 };
  constexpr static const unsigned int SIZE_BODY_2 { 14 };

  std::shared_ptr<Font> h1;
  std::shared_ptr<Font> h2;
  std::shared_ptr<Font> h3;
  std::shared_ptr<Font> h4;
  std::shared_ptr<Font> h5;
  std::shared_ptr<Font> h6;
  std::shared_ptr<Font> body1;
  std::shared_ptr<Font> body2;
};

struct Components
{
  struct Text
  {
    Core::Color color { Palette::WHITE_ACCENT };
  } text;

  struct TextButton
  {
    Text active { .color { Palette::WHITE } };
    Text hover { .color { Palette::WHITE_ACCENT } };
    Text disabled { .color { Palette::GRAY } };
  } text_button;

  struct MenuScreen
  {
    Core::Color background_color { 0x00'00'00'bb }; // NOLINT
  } menu_screen;
};

struct Theme
{
  Typography typography;
  Components components;
  Palette palette;
};

}
