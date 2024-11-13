#pragma once
#include <SDL_surface.h>
#include <SDL_ttf.h>
#include <memory>
#include <string>
#include <tuple>

#include "src/utils/data_holder.h"
#include "src/utils/types.h"

namespace GUI {

class Font
{
private:
  using FontBearer = std::unique_ptr<TTF_Font, void (*)(TTF_Font*)>;

  static void font_pointer_deleter(TTF_Font* pointer);
  static void surface_pointer_deleter(SDL_Surface* pointer);

  std::shared_ptr<DataHolder> _font_data_holder;
  FontBearer _font { nullptr, Font::font_pointer_deleter };
  int _size;

public:
  Font(const std::shared_ptr<DataHolder>& font_data_holder, int size);
  Font(const Font&) = delete;
  Font(Font&&) noexcept = delete;
  Font& operator=(const Font&) = delete;
  Font& operator=(Font&&) noexcept = delete;
  ~Font() = default;

  using SurfaceBearer = std::unique_ptr<SDL_Surface, void (*)(SDL_Surface*)>;

  Font::SurfaceBearer draw(const std::string& text, const Core::Color& color);
  [[nodiscard]] int size() const;

  std::tuple<int, int> calculate_size(const std::string& value);
};

}
