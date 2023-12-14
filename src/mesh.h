#pragma once
#include <GL/glew.h>
#include <memory>
#include <tiny_gltf.h>


class Mesh {
private:
  GLuint _vao = 0; // vertex array object
  GLuint _vbo = 0; // vertex buffer object
  GLuint _nbo = 0; // normal buffer object
  GLuint _ebo = 0; // element buffer object
  GLuint _tbo = 0; // texture coordinates buffer object

  int _mode = GL_TRIANGLES;
  size_t _count;
  int _componentType = GL_UNSIGNED_INT;

  void free();

public:
  explicit Mesh(tinygltf::Model &model);
  explicit Mesh(
    std::vector<float>& vertices,
    std::vector<float>& normals,
    std::vector<float>& tex_coords,
    std::vector<unsigned int>& indices
  );

  ~Mesh();

  void draw();

  static std::unique_ptr<Mesh> quad(float z = 0.0);
};
