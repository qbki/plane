#include <SDL_stdinc.h>
#include <SDL_surface.h>
#include <SDL_ttf.h>
#include <memory>
#include <stdexcept>

#include "src/utils/file_loaders.h"
#include "src/utils/types.h"

#include "font.h"

namespace GUI {

void
Font::font_pointer_deleter(TTF_Font* pointer)
{
  TTF_CloseFont(pointer);
};

void
Font::surface_pointer_deleter(SDL_Surface* pointer)
{
  SDL_FreeSurface(pointer);
};

Font::Font(const std::shared_ptr<RWopsHolder>& font_rw, int size)
  : _font_rw(font_rw)
  , _size(size)
{
  auto font = TTF_OpenFontRW(_font_rw->rwops.get(), SDL_FALSE, _size);
  if (font == nullptr) {
    throw std::runtime_error(TTF_GetError());
  }
  _font = { font, Font::font_pointer_deleter };
}

Font::SurfaceBearer
Font::draw(const std::string& text, const Core::Color& color)
{
  auto font_surface = TTF_RenderUTF8_Blended(
    _font.get(), text.c_str(), { color.r, color.g, color.b, color.a });
  if (font_surface == nullptr) {
    throw std::runtime_error(TTF_GetError());
  }
  return { font_surface, surface_pointer_deleter };
}

int
Font::size() const
{
  return _size;
}

}
