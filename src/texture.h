#pragma once
#include <string>
#include <vector>
#include <GL/gl.h>


class Texture {
public:
  enum class Type {
    MAIN,
    DESTROYED
  };
  static Type DefaultType;

private:
  Type _type;
  GLuint to = 0; // texture object

public:
  Texture(Type type,  const std::vector<unsigned char>& data);
  ~Texture();

  void use(unsigned int num);

  Type type();
  void type(Type name);

  static std::string map_to_str(Type type);
  static Type map_to_type(std::string& type);
};

inline Texture::Type Texture::DefaultType = Texture::Type::MAIN;
