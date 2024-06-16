#include <GL/glew.h>
#include <cassert>
#include <stdexcept>
#include <utility>

#include "texture.h"

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
      throw std::runtime_error("Wrong texture index");
  }
}

/**
 * @param data Expects 4 bytes per pixel (RGBA) and a rectangular texture
 */
Texture::Texture(int width, int height, const std::vector<unsigned char>& data)
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
}

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
  _texture_object =
    std::exchange(other._texture_object, DEFAULT_TEXTURE_OBJECT);
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

void
Texture::data(const std::vector<unsigned char>& pixels)
{
  glBindTexture(GL_TEXTURE_2D, _texture_object);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexSubImage2D(GL_TEXTURE_2D,
                  0,
                  0,
                  0,
                  _width,
                  _height,
                  GL_RGBA,
                  GL_UNSIGNED_BYTE,
                  pixels.data());
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void
Texture::data(const std::vector<unsigned char>& pixels, int width, int height)
{
  _width = width;
  _height = height;
  glBindTexture(GL_TEXTURE_2D, _texture_object);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_SRGB8_ALPHA8,
               width,
               height,
               0,
               GL_RGBA,
               GL_UNSIGNED_BYTE,
               pixels.data());
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  glBindTexture(GL_TEXTURE_2D, 0);
}
