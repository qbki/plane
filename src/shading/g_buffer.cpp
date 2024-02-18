#include <GL/glew.h>
#include <array>
#include <format>
#include <stdexcept>

#include "src/services.h"

#include "g_buffer.h"

const std::array<GLuint, 3> GBuffer::COLOR_ATTACHMENTS = {
  GL_COLOR_ATTACHMENT0,
  GL_COLOR_ATTACHMENT1,
  GL_COLOR_ATTACHMENT2,
};

void
gen_color_attachment(GLuint& texture_handle,
                     GLint internal_color_format,
                     GLint color_attachment,
                     unsigned int width,
                     unsigned int height,
                     GLuint date_type)
{
  glGenTextures(1, &texture_handle);
  glBindTexture(GL_TEXTURE_2D, texture_handle);
  glTexImage2D(GL_TEXTURE_2D,
               0,
               internal_color_format,
               static_cast<GLint>(width),
               static_cast<GLint>(height),
               0,
               GL_RGBA,
               date_type,
               nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glFramebufferTexture2D(
    GL_DRAW_FRAMEBUFFER, color_attachment, GL_TEXTURE_2D, texture_handle, 0);
}

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
  gen_color_attachment(_position_texture,
                       GL_RGBA32F,
                       GL_COLOR_ATTACHMENT0,
                       _width,
                       _height,
                       GL_FLOAT);
  gen_color_attachment(_normal_texture,
                       GL_RGBA32F,
                       GL_COLOR_ATTACHMENT1,
                       _width,
                       _height,
                       GL_FLOAT);
  gen_color_attachment(_base_color_texture,
                       GL_RGBA8,
                       GL_COLOR_ATTACHMENT2,
                       _width,
                       _height,
                       GL_UNSIGNED_BYTE);
  glDrawBuffers(3, GBuffer::COLOR_ATTACHMENTS.data());

  glGenRenderbuffers(1, &_render_buffer);
  glBindRenderbuffer(GL_RENDERBUFFER, _render_buffer);
  glRenderbufferStorage(GL_RENDERBUFFER,
                        GL_DEPTH24_STENCIL8,
                        static_cast<GLsizei>(_width),
                        static_cast<GLsizei>(_height));
  glFramebufferRenderbuffer(
    GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _render_buffer);

  auto status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    std::array<GLint, 4> dimentions{ 0, 0, 0, 0 };
    glGetIntegerv(GL_VIEWPORT, dimentions.data());
    logger().error(std::format(
      "Viewport dimentions: {}x{}", dimentions.at(2), dimentions.at(3)));
    auto status_text =
      std::format("Framebuffer status ({}x{}): {}", _width, _height, status);
    throw std::runtime_error(status_text);
  }

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
