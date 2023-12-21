#pragma once
#include <GL/glew.h>
#include <array>

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
  static const std::array<GLuint, 3> COLOR_ATTACHMENTS;

  GBuffer();
  GBuffer(const GBuffer&) = delete;
  GBuffer& operator=(const GBuffer&) = delete;
  GBuffer(GBuffer&&) = delete;
  GBuffer& operator=(GBuffer&&) = delete;
  ~GBuffer();

  void update(unsigned int width, unsigned int height);
  void use();
  [[nodiscard]] GLuint g_buffer_handle() const;
};
