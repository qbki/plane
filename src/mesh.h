#pragma once
#include <GL/glew.h>
#include <memory>
#include <tiny_gltf.h>

class Mesh
{
private:
  GLuint _vertex_array_object = 0;
  GLuint _vertex_buffer_object = 0;
  GLuint _normal_buffer_object = 0;
  GLuint _element_buffer_object = 0;
  GLuint _texcoord_buffer_object = 0;

  int _mode = GL_TRIANGLES;
  size_t _count;
  int _componentType = GL_UNSIGNED_INT;

  void free();

public:
  explicit Mesh(tinygltf::Model& model);
  Mesh(std::vector<float>& vertices,
       std::vector<float>& normals,
       std::vector<float>& tex_coords,
       std::vector<unsigned int>& indices);
  Mesh(const Mesh&) = delete;
  Mesh& operator=(const Mesh&) = delete;
  Mesh(Mesh&&) = delete;
  Mesh& operator=(Mesh&&) = delete;
  ~Mesh();

  void draw();

  static std::unique_ptr<Mesh> quad(float z = 0.0);
};
