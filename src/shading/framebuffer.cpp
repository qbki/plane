#include <GL/glew.h>
#include <array>

#include "src/shading/framebuffer.h"
#include "src/utils/gl.h"

const GLuint FrameBuffer::COLOR_ATTACHMENT = GL_COLOR_ATTACHMENT0;

FrameBuffer::FrameBuffer()
{
  glGenFramebuffers(1, &_frame_buffer);
}

FrameBuffer::~FrameBuffer()
{
  glDeleteTextures(1, &_texture);
  glDeleteRenderbuffers(1, &_render_buffer);
  glDeleteFramebuffers(1, &_frame_buffer);
}

void
FrameBuffer::update(unsigned int width, unsigned int height)
{
  if (width == _width && height == _height) {
    return;
  }
  _width = width;
  _height = height;
  glDeleteTextures(1, &_texture);
  glDeleteRenderbuffers(1, &_render_buffer);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _frame_buffer);
  _texture = gen_color_attachment(
    GL_RGBA32F, GL_COLOR_ATTACHMENT0, _width, _height, GL_FLOAT);
  GLuint attachment = GL_COLOR_ATTACHMENT0;
  glDrawBuffers(1, &attachment);
  _render_buffer = gen_render_buffer(_width, _height);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void
FrameBuffer::use()
{
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _texture);
}

GLuint
FrameBuffer::handle() const
{
  return _frame_buffer;
}
