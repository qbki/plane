#pragma once
#include <GL/glew.h>

class GBuffer {
private:
  GLuint _g_frame_buffer = 0;
  GLuint _position_texture = 0;
  GLuint _normal_texture = 0;
  GLuint _base_color_texture = 0;
  GLuint _render_buffer = 0;
  unsigned int _width = 0;
  unsigned int _height = 0;

  constexpr static GLuint COLOR_ATTACHMENTS[3] = {
    GL_COLOR_ATTACHMENT0,
    GL_COLOR_ATTACHMENT1,
    GL_COLOR_ATTACHMENT2,
  };

public:
  GBuffer();
  ~GBuffer();

  void update(unsigned int width, unsigned int height);
  void use();
  GLuint g_buffer_handle() const;
};
