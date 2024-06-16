#pragma once
#include <GL/glew.h>
#include <vector>

class Texture
{
private:
  static const GLuint DEFAULT_TEXTURE_OBJECT;

  int _width = 0;
  int _height = 0;
  GLuint _texture_object = DEFAULT_TEXTURE_OBJECT;

public:
  Texture() = default;
  explicit Texture(int width,
                   int height,
                   const std::vector<unsigned char>& data);
  Texture(const Texture&) = delete;
  Texture& operator=(const Texture&) = delete;
  Texture(Texture&&) noexcept;
  Texture& operator=(Texture&&) noexcept;
  ~Texture() noexcept;

  [[nodiscard]] int width() const;
  [[nodiscard]] int height() const;

  void use(unsigned int idx) const;
  void data(const std::vector<unsigned char>& pixels);
  void data(const std::vector<unsigned char>& pixels, int width, int height);
};
