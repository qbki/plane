#pragma once
#include <cstdint>
#include <filesystem>
#include <memory>
#include <thorvg.h>
#include <tuple>
#include <vector>

#include "src/texture.h"

class UiCanvas
{
private:
  static constexpr tvg::SwCanvas::Colorspace COLOR_SPACE {
    tvg::SwCanvas::Colorspace::ARGB8888
  };

  int _width = 0;
  int _height = 0;
  std::vector<uint32_t> _target;
  std::unique_ptr<tvg::SwCanvas> _canvas;
  std::unique_ptr<Texture> _texture;

public:
  UiCanvas(int init_width, int init_height);

  UiCanvas(const UiCanvas&) = delete;
  UiCanvas(UiCanvas&&) = default;
  UiCanvas& operator=(const UiCanvas&) = delete;
  UiCanvas& operator=(UiCanvas&&) = default;
  ~UiCanvas() = default;

  [[nodiscard]] int width() const;
  [[nodiscard]] int height() const;

  [[nodiscard]] const std::vector<uint32_t>& target() const;
  tvg::Canvas& canvas();
  Texture& texture();

  void draw();

  void clear();

  void save_to_file(const std::filesystem::path& path);

private:
  static std::tuple<int, int> get_valid_size(int width, int height);
};
