#pragma once
#include <GL/glew.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <memory>
#include <tiny_gltf.h>

#include "src/game_state/texture_type.h"

class Mesh
{
public:
  struct DrawParams
  {
    std::vector<glm::mat4> transforms;
    std::vector<TextureType::Type> texture_indices;
  };

private:
  GLuint _vertex_array_object = 0;
  GLuint _vertex_buffer_object = 0;
  GLuint _normal_buffer_object = 0;
  GLuint _transform_buffer_object = 0;
  GLuint _texture_indices_buffer_object = 0;
  GLuint _element_buffer_object = 0;
  GLuint _texcoord_buffer_object = 0;

  int _mode = GL_TRIANGLES;
  size_t _number_of_elements;
  size_t _instance_quantity = 1;
  int _componentType = GL_UNSIGNED_INT;

  void free();

public:
  explicit Mesh(tinygltf::Model& model, size_t instance_quantity = 1);
  Mesh(std::vector<float>& vertices,
       std::vector<float>& normals,
       std::vector<float>& tex_coords,
       std::vector<unsigned int>& indices,
       size_t instance_quantity = 1);
  Mesh(const Mesh&) = delete;
  Mesh& operator=(const Mesh&) = delete;
  Mesh(Mesh&&) = delete;
  Mesh& operator=(Mesh&&) = delete;
  ~Mesh();

  void draw(const DrawParams& draw_params);

  static std::unique_ptr<Mesh> quad(float z = 0.0);
};
