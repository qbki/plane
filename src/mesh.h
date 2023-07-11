#pragma once
#include <GL/glew.h>
#include <GL/gl.h>
#include <tiny_gltf.h>

class Mesh {
private:
  GLuint vao = 0; // vertex array object
  GLuint vbo = 0; // vertex buffer object
  GLuint ebo = 0; // element buffer object

  int mode;
  size_t count;
  int componentType;

  void free();

public:
  Mesh(tinygltf::Model &model);
  ~Mesh();
  void draw();
};
