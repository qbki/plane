#pragma once
#include <GL/glew.h>
#include <vector>

class Texture
{
private:
  static const unsigned int DEFAULT_TEXTURE_TYPE;
  static const GLuint DEFAULT_TEXTURE_OBJECT;

  unsigned int _id = DEFAULT_TEXTURE_TYPE;
  GLuint _texture_object = DEFAULT_TEXTURE_OBJECT;

public:
  Texture() = default;
  explicit Texture(unsigned int id, const std::vector<unsigned char>& data);
  Texture(const Texture&) = delete;
  Texture& operator=(const Texture&) = delete;
  Texture(Texture&&) noexcept;
  Texture& operator=(Texture&&) noexcept;
  virtual ~Texture() noexcept;

  void use(unsigned int idx) const;

  [[nodiscard]] GLuint id() const;
  void id(unsigned int type_id);
};
