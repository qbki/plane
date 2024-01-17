#pragma once
#include <GL/glew.h>
#include <vector>

class Texture
{
private:
  static const unsigned int DEFAULT_TEXTURE_TYPE;
  static const GLuint DEFAULT_TEXTURE_OBJECT;

  unsigned int _type = DEFAULT_TEXTURE_TYPE;
  GLuint _texture_object = DEFAULT_TEXTURE_OBJECT;

public:
  explicit Texture(unsigned int type, const std::vector<unsigned char>& data);
  Texture(const Texture&) = delete;
  Texture& operator=(const Texture&) = delete;
  Texture(Texture&&) noexcept;
  Texture& operator=(Texture&&) noexcept;
  virtual ~Texture() noexcept;

  void use(unsigned int idx);

  [[nodiscard]] GLuint type() const;
  void type(unsigned int type_id);
};
