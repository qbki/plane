#pragma once
#include <filesystem>

import utils.color;

namespace GUI {

struct Palette
{
  constexpr static pln::utils::color::Color WHITE { 0x7f'7d'7e'ff };
  constexpr static pln::utils::color::Color WHITE_ACCENT { 0xff'fd'fe'ff };
  constexpr static pln::utils::color::Color GRAY { 0x43'43'43'ff };
  constexpr static pln::utils::color::Color RED { 0xcb'00'66'ff };

  pln::utils::color::Color white { WHITE };
  pln::utils::color::Color white_accent { WHITE_ACCENT };
  pln::utils::color::Color gray { GRAY };
};

struct Typography
{
  struct Font
  {
    std::string name {};
    float size { 1 };
  };

  constexpr static unsigned int SIZE_H1 { 96 };
  constexpr static unsigned int SIZE_H2 { 60 };
  constexpr static unsigned int SIZE_H3 { 48 };
  constexpr static unsigned int SIZE_H4 { 34 };
  constexpr static unsigned int SIZE_H5 { 24 };
  constexpr static unsigned int SIZE_H6 { 20 };
  constexpr static unsigned int SIZE_BODY_1 { 16 };
  constexpr static unsigned int SIZE_BODY_2 { 14 };

  Font h1 { .size = SIZE_H1 };
  Font h2 { .size = SIZE_H2 };
  Font h3 { .size = SIZE_H3 };
  Font h4 { .size = SIZE_H4 };
  Font h5 { .size = SIZE_H5 };
  Font h6 { .size = SIZE_H6 };
  Font body1 { .size = SIZE_BODY_1 };
  Font body2 { .size = SIZE_BODY_2 };
};

struct Components
{
  struct Text
  {
    pln::utils::color::Color color { Palette::WHITE_ACCENT };
  } text;

  struct TextButton
  {
    Text active { .color { Palette::WHITE } };
    Text hover { .color { Palette::WHITE_ACCENT } };
    Text disabled { .color { Palette::GRAY } };
  } text_button;

  struct MenuScreen
  {
    pln::utils::color::Color background_color { 0x00'00'00'bb }; // NOLINT
  } menu_screen;
};

struct Theme
{
  Typography typography;
  Components components;
  Palette palette;
  std::filesystem::path font_path {};
};

}
