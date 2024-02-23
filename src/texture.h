#pragma once
#include <GL/glew.h>
#include <vector>

class Texture
{
private:
  static const GLuint DEFAULT_TEXTURE_OBJECT;

  GLuint _texture_object = DEFAULT_TEXTURE_OBJECT;

public:
  Texture() = default;
  explicit Texture(const std::vector<unsigned char>& data);
  Texture(const Texture&) = delete;
  Texture& operator=(const Texture&) = delete;
  Texture(Texture&&) noexcept;
  Texture& operator=(Texture&&) noexcept;
  virtual ~Texture() noexcept;

  void use(unsigned int idx) const;
};
