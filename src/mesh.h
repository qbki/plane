#pragma once
#include <GL/gl.h>
#include <tiny_gltf.h>

class Mesh {
private:
  GLuint vao = 0; // vertex array object
  GLuint vbo = 0; // vertex buffer object
  GLuint nbo = 0; // normal buffer object
  GLuint ebo = 0; // element buffer object
  GLuint tbo = 0; // texture coordinates buffer object

  int mode;
  size_t count;
  int componentType;

  void free();

public:
  explicit Mesh(tinygltf::Model &model);
  ~Mesh();
  void draw();
};
