#include <GL/glew.h>
#include <format>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>

#include "../utils.h"
#include "g_buffer.h"
#include "../services.h"


void gen_color_attachment(
  GLuint& texture_handle,
  GLuint internal_color_format,
  unsigned int attachment_index,
  unsigned int width,
  unsigned int height,
  GLuint date_type
) {
  auto attachment = GL_COLOR_ATTACHMENT0;
  switch (attachment_index) {
    case 1: {
      attachment = GL_COLOR_ATTACHMENT1;
      break;
    }
    case 2: {
      attachment = GL_COLOR_ATTACHMENT2;
      break;
    }
  }

  glGenTextures(1, &texture_handle);
  glBindTexture(GL_TEXTURE_2D, texture_handle);
  glTexImage2D(GL_TEXTURE_2D, 0, internal_color_format, width, height, 0, GL_RGBA, date_type, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture_handle, 0);
}


GBuffer::GBuffer() {
  glGenFramebuffers(1, &_g_frame_buffer);
}


GBuffer::~GBuffer() {
  glDeleteTextures(1, &_base_color_texture);
  glDeleteTextures(1, &_normal_texture);
  glDeleteTextures(1, &_position_texture);
  glDeleteRenderbuffers(1, &_render_buffer);
  glDeleteFramebuffers(1, &_g_frame_buffer);
}


void GBuffer::update(unsigned int width, unsigned int height) {
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
  gen_color_attachment(_position_texture, GL_RGBA32F, 0, _width, _height, GL_FLOAT);
  gen_color_attachment(_normal_texture, GL_RGBA32F, 1, _width, _height, GL_FLOAT);
  gen_color_attachment(_base_color_texture, GL_RGBA8, 2, _width, _height, GL_UNSIGNED_BYTE);
  glDrawBuffers(3, GBuffer::COLOR_ATTACHMENTS);

  glGenRenderbuffers(1, &_render_buffer);
  glBindRenderbuffer(GL_RENDERBUFFER, _render_buffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, _width, _height);
  glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _render_buffer);

  auto status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    GLint dims[4] = {0};
    glGetIntegerv(GL_VIEWPORT, dims);
    logger().error(std::format("Viewport dimentions: {}x{}", dims[2], dims[3]));
    auto status_text = std::format("Framebuffer status ({}x{}): {}", _width, _height, status);
    throw std::runtime_error(status_text);
  }

  glViewport(0, 0, width, height);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}


void GBuffer::use() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _position_texture);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, _normal_texture);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, _base_color_texture);
}


GLuint GBuffer::g_buffer_handle() const {
  return _g_frame_buffer;
}
