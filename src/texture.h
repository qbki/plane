#pragma once
#include <string>
#include <vector>
#include <GL/gl.h>


class Texture {
private:
  unsigned int _type;
  GLuint to = 0; // texture object

public:
  Texture(unsigned int type, const std::vector<unsigned char>& data);
  ~Texture();

  void use(unsigned int num);

  unsigned int type();
  void type(unsigned int type);
};
