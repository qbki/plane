#pragma once
#include <vector>
#include <GL/gl.h>


class Texture {
private:
  GLuint to = 0; // texture object

public:
  Texture(const std::vector<unsigned char>& data);
  ~Texture();
  void use(unsigned int num);
};
