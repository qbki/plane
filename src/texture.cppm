module;
#include <GL/glew.h>
#include <cassert>
#include <cstdint>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

export module pln.textures;

import pln.utils.crash;
import pln.utils.gl;

namespace pln::textures {

export
template<typename T>
concept TextureData = std::is_same_v<T, unsigned char>
                      || std::is_same_v<T, uint32_t>;


export class Texture
{
private:
  static constexpr GLuint DEFAULT_TEXTURE_OBJECT = 0;

  int _width;
  int _height;
  GLuint _texture_object = DEFAULT_TEXTURE_OBJECT;

public:
  Texture(const Texture&) = delete;
  Texture& operator=(const Texture&) = delete;


  /**
   * @param data Expects 4 bytes per pixel (RGBA) and a rectangular texture
   */
  Texture(Texture&& other) noexcept
    : _width(other._width)
    , _height(other._height)
    , _texture_object(
        std::exchange(other._texture_object, DEFAULT_TEXTURE_OBJECT))
  {
  }


  Texture&
  operator=(Texture&& other) noexcept
  {
    if (this == &other) {
      return *this;
    }
    _width = other._width;
    _height = other._height;
    _texture_object = std::exchange(other._texture_object,
                                    DEFAULT_TEXTURE_OBJECT);
    return *this;
  }


  ~Texture() noexcept
  {
    if (_texture_object != DEFAULT_TEXTURE_OBJECT) {
      glDeleteTextures(1, &_texture_object);
    }
  }


  template<typename T>
    requires TextureData<T>
  Texture(int width, int height, const std::vector<T>& data)
    : _width(width)
    , _height(height)
  {
    assert(width > 0 && "Width of a texture should be more than 0");
    assert(height > 0 && "Height of a texture should be more than 0");

    glGenTextures(1, &_texture_object);
    glBindTexture(GL_TEXTURE_2D, _texture_object);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_SRGB8_ALPHA8,
                 width,
                 height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 data.data());
    glBindTexture(GL_TEXTURE_2D, 0);
    utils::print_opengl_errors("Texture Initialization");
  }


  [[nodiscard]] GLuint
  int_to_texture_index(unsigned int idx) const
  {
    switch (idx) {
      case 0:
        return GL_TEXTURE0;
      case 1:
        return GL_TEXTURE1;
      case 2:
        return GL_TEXTURE2;
      case 3:
        return GL_TEXTURE3;
      case 4:
        return GL_TEXTURE4;
      case 5: // NOLINT(cppcoreguidelines-avoid-magic-numbers)
        return GL_TEXTURE5;
      default:
        pln::utils::crash("Wrong texture index");
    }
  }


  [[nodiscard]] int
  width() const
  {
    return _width;
  }


  [[nodiscard]] int
  height() const
  {
    return _height;
  }


  void
  use(unsigned int idx) const
  {
    glActiveTexture(int_to_texture_index(idx));
    glBindTexture(GL_TEXTURE_2D, _texture_object);
  }


  template<typename T>
    requires TextureData<T>
  void
  data(const std::vector<T>& pixels)
  {
    data(pixels, _width, _height);
  }


  template<typename T>
    requires TextureData<T>
  void
  data(const std::vector<T>& pixels, int width, int height)
  {
    assert(width > 0 && "Width of a texture should be more than 0");
    assert(height > 0 && "Height of a texture should be more than 0");

    _width = width;
    _height = height;
    glBindTexture(GL_TEXTURE_2D, _texture_object);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_SRGB8_ALPHA8,
                 width,
                 height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 pixels.data());
    glBindTexture(GL_TEXTURE_2D, 0);
    utils::print_opengl_errors("Texture Initialization");
  }


  template<> void
  data<uint32_t>(const std::vector<uint32_t>& pixels);


  template<> void
  data<unsigned char>(const std::vector<unsigned char>& pixels);


  template<> void
  data<unsigned char>(const std::vector<unsigned char>& pixels,
                      int width,
                      int height);

};

}
