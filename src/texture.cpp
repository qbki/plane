#include <GL/gl.h>
#include <GL/glext.h>
#include <cmath>
#include <stdexcept>

#include "texture.h"
#include "utils.h"


GLuint int_to_texture_index(unsigned int idx) {
  switch (idx) {
    case 0: return GL_TEXTURE0;
    case 1: return GL_TEXTURE1;
    case 2: return GL_TEXTURE2;
    case 3: return GL_TEXTURE3;
    case 4: return GL_TEXTURE4;
    case 5: return GL_TEXTURE5;
    default: throw std::runtime_error("Wrong texture index");
  }
}


/**
 * @param data Expects 4 bytes per pixel (RGBA) and a rectangular texture
 */
Texture::Texture(
    unsigned int type,
    const std::vector<unsigned char>& data
) : _type(type) {
  size_t size = std::sqrt(data.size() / 4);
  glGenTextures(1, &this->to);
  glBindTexture(GL_TEXTURE_2D, this->to);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGBA,
    size,
    size,
    0,
    GL_RGBA,
    GL_UNSIGNED_BYTE,
    data.data()
  );
}


Texture::~Texture() {
  glDeleteTextures(1, &this->to);
}


void Texture::use(unsigned int idx) {
  glActiveTexture(int_to_texture_index(idx));
  glBindTexture(GL_TEXTURE_2D, this->to);
}


unsigned int Texture::type() {
  return this->_type;
}


void Texture::type(unsigned int value) {
  this->_type = value;
}
