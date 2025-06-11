#pragma once
#include <GL/glew.h>
#include <cassert>
#include <cstdint>
#include <string>
#include <type_traits>
#include <vector>

#include "src/utils/gl.h"

template<typename T>
concept TextureData = std::is_same_v<T, unsigned char>
                      || std::is_same_v<T, uint32_t>;

class Texture
{
private:
  static const GLuint DEFAULT_TEXTURE_OBJECT;

  int _width;
  int _height;
  GLuint _texture_object = DEFAULT_TEXTURE_OBJECT;

public:
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
    print_opengl_errors("Texture Initialization");
  }

  Texture(const Texture&) = delete;
  Texture(Texture&&) noexcept;
  Texture& operator=(const Texture&) = delete;
  Texture& operator=(Texture&&) noexcept;
  ~Texture() noexcept;

  [[nodiscard]] int width() const;
  [[nodiscard]] int height() const;

  void use(unsigned int idx) const;

  template<typename T>
    requires TextureData<T>
  void data(const std::vector<T>& pixels);

  template<typename T>
    requires TextureData<T>
  void data(const std::vector<T>& pixels, int width, int height);
};
