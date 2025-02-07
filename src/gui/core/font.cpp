#include <SDL_pixels.h>
#include <SDL_stdinc.h>
#include <SDL_surface.h>
#include <SDL_ttf.h>
#include <format>
#include <memory>

#include "src/utils/crash.h"

#include "font.h"

namespace GUI {

void
Font::font_pointer_deleter(TTF_Font* pointer)
{
  if (TTF_WasInit() > 0) {
    TTF_CloseFont(pointer);
  }
};

void
Font::surface_pointer_deleter(SDL_Surface* pointer)
{
  SDL_FreeSurface(pointer);
};

Font::Font(const std::shared_ptr<DataHolder>& font_data_holder, int size)
  : _font_data_holder(font_data_holder)
  , _size(size)
{
  auto unsafe_rwops = _font_data_holder->unsafe_rwops();
  auto font = TTF_OpenFontRW(unsafe_rwops, SDL_TRUE, _size);
  if (font == nullptr) {
    crash(TTF_GetError());
  }
  _font = { font, Font::font_pointer_deleter };
}

Font::SurfaceBearer
Font::draw(const std::string& text, const Core::Color& color)
{
  SDL_Color sdl_color = {
    .r = color.r,
    .g = color.g,
    .b = color.b,
    .a = color.a,
  };
  auto font_surface = TTF_RenderUTF8_Blended(
    _font.get(), text.c_str(), sdl_color);
  if (font_surface == nullptr) {
    crash(TTF_GetError());
  }
  return { font_surface, surface_pointer_deleter };
}

int
Font::size() const
{
  return _size;
}

std::tuple<int, int>
Font::calculate_size(const std::string& value)
{
  int width { 0 };
  int height { 0 };
  auto error = TTF_SizeUTF8(_font.get(), value.c_str(), &width, &height);
  if (error < 0) {
    crash(std::format("Can't measure text: {}", value));
  }
  return { width, height };
}

}
