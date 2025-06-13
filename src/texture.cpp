#include <GL/glew.h>
#include <cassert>
#include <utility>

#include "src/utils/gl.h"

#include "texture.h"

import utils.crash;

const GLuint Texture::DEFAULT_TEXTURE_OBJECT = 0;

GLuint
int_to_texture_index(unsigned int idx)
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
      utils::crash("Wrong texture index");
  }
}

/**
 * @param data Expects 4 bytes per pixel (RGBA) and a rectangular texture
 */
Texture::Texture(Texture&& other) noexcept
  : _width(other._width)
  , _height(other._height)
  , _texture_object(
      std::exchange(other._texture_object, DEFAULT_TEXTURE_OBJECT))
{
}

Texture&
Texture::operator=(Texture&& other) noexcept
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

Texture::~Texture() noexcept
{
  if (_texture_object != DEFAULT_TEXTURE_OBJECT) {
    glDeleteTextures(1, &_texture_object);
  }
}

int
Texture::width() const
{
  return _width;
}

int
Texture::height() const
{
  return _height;
}

void
Texture::use(unsigned int idx) const
{
  glActiveTexture(int_to_texture_index(idx));
  glBindTexture(GL_TEXTURE_2D, _texture_object);
}

template<typename T>
  requires TextureData<T>
void
Texture::data(const std::vector<T>& pixels)
{
  data(pixels, _width, _height);
}

template void
Texture::data<uint32_t>(const std::vector<uint32_t>& pixels);

template void
Texture::data<unsigned char>(const std::vector<unsigned char>& pixels);

template<typename T>
  requires TextureData<T>
void
Texture::data(const std::vector<T>& pixels, int width, int height)
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
  print_opengl_errors("Texture Initialization");
}

template void
Texture::data<uint32_t>(const std::vector<uint32_t>& pixels,
                        int width,
                        int height);

template void
Texture::data<unsigned char>(const std::vector<unsigned char>& pixels,
                             int width,
                             int height);
