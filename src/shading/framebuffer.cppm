module;
#include <GL/glew.h>

#include "src/utils/gl.h"

export module pln.shading.framebuffer;

namespace pln::shading {

export
class FrameBuffer
{
private:
  GLuint _frame_buffer = 0;
  GLuint _render_buffer = 0;
  GLuint _texture = 0;
  unsigned int _width = 0;
  unsigned int _height = 0;

  static constexpr GLuint COLOR_ATTACHMENT = GL_COLOR_ATTACHMENT0;

public:
  FrameBuffer(const FrameBuffer&) = delete;
  FrameBuffer& operator=(const FrameBuffer&) = delete;
  FrameBuffer(FrameBuffer&&) = delete;
  FrameBuffer& operator=(FrameBuffer&&) = delete;

  FrameBuffer()
  {
    glGenFramebuffers(1, &_frame_buffer);
  }


  ~FrameBuffer()
  {
    glDeleteTextures(1, &_texture);
    glDeleteRenderbuffers(1, &_render_buffer);
    glDeleteFramebuffers(1, &_frame_buffer);
  }


  void
  update(unsigned int width, unsigned int height)
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
  use()
  {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture);
  }


  [[nodiscard]] GLuint
  handle() const
  {
    return _frame_buffer;
  }
};

}
