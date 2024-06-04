#include <GL/glew.h>
#include <array>

#include "src/shading/g_buffer.h"
#include "src/utils/gl.h"

const std::array<GLuint, 3> GBuffer::COLOR_ATTACHMENTS = {
  GL_COLOR_ATTACHMENT0,
  GL_COLOR_ATTACHMENT1,
  GL_COLOR_ATTACHMENT2,
};

GBuffer::GBuffer()
{
  glGenFramebuffers(1, &_g_frame_buffer);
}

GBuffer::~GBuffer()
{
  glDeleteTextures(1, &_base_color_texture);
  glDeleteTextures(1, &_normal_texture);
  glDeleteTextures(1, &_position_texture);
  glDeleteRenderbuffers(1, &_render_buffer);
  glDeleteFramebuffers(1, &_g_frame_buffer);
}

void
GBuffer::update(unsigned int width, unsigned int height)
{
  if (width == _width && height == _height) {
    return;
  }
  _width = width;
  _height = height;
  glDeleteTextures(1, &_base_color_texture);
  glDeleteTextures(1, &_normal_texture);
  glDeleteTextures(1, &_position_texture);
  glDeleteRenderbuffers(1, &_render_buffer);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _g_frame_buffer);
  _position_texture = gen_color_attachment(
    GL_RGBA32F, GL_COLOR_ATTACHMENT0, _width, _height, GL_FLOAT);
  _normal_texture = gen_color_attachment(
    GL_RGBA32F, GL_COLOR_ATTACHMENT1, _width, _height, GL_FLOAT);
  _base_color_texture = gen_color_attachment(
    GL_RGBA8, GL_COLOR_ATTACHMENT2, _width, _height, GL_UNSIGNED_BYTE);
  glDrawBuffers(3, GBuffer::COLOR_ATTACHMENTS.data());
  _render_buffer = gen_render_buffer(_width, _height);
  glViewport(0, 0, static_cast<GLsizei>(_width), static_cast<GLsizei>(_height));
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void
GBuffer::use()
{
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _position_texture);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, _normal_texture);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, _base_color_texture);
}

GLuint
GBuffer::g_buffer_handle() const
{
  return _g_frame_buffer;
}
