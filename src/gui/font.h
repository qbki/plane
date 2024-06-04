#pragma once
#include <SDL_surface.h>
#include <SDL_ttf.h>
#include <memory>
#include <string>
#include <vector>

#include "src/utils/types.h"

namespace GUI {

class Font
{
private:
  using FontBearer = std::unique_ptr<TTF_Font, void (*)(TTF_Font*)>;

  static void font_pointer_deleter(TTF_Font* pointer);
  static void surface_pointer_deleter(SDL_Surface* pointer);

  FontBearer _font = { nullptr, Font::font_pointer_deleter };
  std::shared_ptr<RWopsHolder> _font_rw;
  int _size;

public:
  Font(const Font&) = delete;
  Font(Font&&) noexcept = delete;
  Font& operator=(const Font&) = delete;
  Font& operator=(Font&&) noexcept = delete;
  ~Font() = default;

  using SurfaceBearer = std::unique_ptr<SDL_Surface, void (*)(SDL_Surface*)>;

  struct DrawResult
  {
    std::vector<unsigned char> data;
    int width;
    int heiht;
  };

  Font(const std::shared_ptr<RWopsHolder>& font_rw, int size);
  Font::SurfaceBearer draw(const std::string& text, const Core::Color& color);
  [[nodiscard]] int size() const;
};

}
