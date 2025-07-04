module;
#include <GL/glew.h>
#include <array>

export module pln.shading.g_buffer;

import pln.utils.gl;

namespace pln::shading {

export
class GBuffer
{
private:
  GLuint _g_frame_buffer = 0;
  GLuint _position_texture = 0;
  GLuint _normal_texture = 0;
  GLuint _base_color_texture = 0;
  GLuint _render_buffer = 0;
  unsigned int _width = 0;
  unsigned int _height = 0;

public:
  static constexpr std::array<GLuint, 3> COLOR_ATTACHMENTS {
    GL_COLOR_ATTACHMENT0,
    GL_COLOR_ATTACHMENT1,
    GL_COLOR_ATTACHMENT2,
  };

  GBuffer(const GBuffer&) = delete;
  GBuffer& operator=(const GBuffer&) = delete;
  GBuffer(GBuffer&&) = delete;
  GBuffer& operator=(GBuffer&&) = delete;

  GBuffer()
  {
    glGenFramebuffers(1, &_g_frame_buffer);
  }


  ~GBuffer()
  {
    glDeleteTextures(1, &_base_color_texture);
    glDeleteTextures(1, &_normal_texture);
    glDeleteTextures(1, &_position_texture);
    glDeleteRenderbuffers(1, &_render_buffer);
    glDeleteFramebuffers(1, &_g_frame_buffer);
  }


  void
  update(unsigned int width, unsigned int height)
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
    _position_texture = utils::gen_color_attachment(
      GL_RGBA32F, GL_COLOR_ATTACHMENT0, _width, _height, GL_FLOAT);
    _normal_texture = utils::gen_color_attachment(
      GL_RGBA32F, GL_COLOR_ATTACHMENT1, _width, _height, GL_FLOAT);
    _base_color_texture = utils::gen_color_attachment(
      GL_RGBA8, GL_COLOR_ATTACHMENT2, _width, _height, GL_UNSIGNED_BYTE);
    glDrawBuffers(3, GBuffer::COLOR_ATTACHMENTS.data());
    _render_buffer = utils::gen_render_buffer(_width, _height);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  }


  void
  use()
  {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _position_texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _normal_texture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _base_color_texture);
  }


  [[nodiscard]] GLuint
  g_buffer_handle() const
  {
    return _g_frame_buffer;
  }
};

}
