#include <GL/glew.h>

class FrameBuffer
{
private:
  GLuint _frame_buffer = 0;
  GLuint _render_buffer = 0;
  GLuint _texture = 0;
  unsigned int _width = 0;
  unsigned int _height = 0;

  static const GLuint COLOR_ATTACHMENT;

public:
  FrameBuffer();
  FrameBuffer(const FrameBuffer&) = delete;
  FrameBuffer& operator=(const FrameBuffer&) = delete;
  FrameBuffer(FrameBuffer&&) = delete;
  FrameBuffer& operator=(FrameBuffer&&) = delete;
  ~FrameBuffer();

  void update(unsigned int width, unsigned int height);
  void use();
  [[nodiscard]] GLuint handle() const;
};
