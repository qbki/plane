#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL_rwops.h>
#include <exception>
#include <iostream>
#include <memory>
#include <span>
#include <stdexcept>
#include <tiny_gltf.h>
#include <tuple>

#include "mesh.h"
#include "utils.h"

const int VERTEX_LOCATION = 0;
const int NORMAL_LOCATION = 1;
const int TEXCOORD_LOCATION = 2;

const auto TEXT_COORD_NAME = "TEXCOORD_0";

auto
extract_data_by_accessor(const tinygltf::Model& model, int accessor_idx)
{
  auto accessor = model.accessors.at(accessor_idx);
  auto bufferView = model.bufferViews.at(accessor.bufferView);
  auto buffer = model.buffers.at(bufferView.buffer);
  return std::make_tuple(bufferView, buffer);
}

template<typename T>
void
create_buffer(GLuint& buffer_handle, GLuint target, std::vector<T>& data)
{
  glGenBuffers(1, &buffer_handle);
  glBindBuffer(target, buffer_handle);
  glBufferData(target, data.size() * sizeof(T), data.data(), GL_STATIC_DRAW);
}

void
create_buffer_from_gltf(GLuint& buffer_handle,
                        const tinygltf::Model& model,
                        int accessor_idx)
{
  auto [buffer_view, buffer] = extract_data_by_accessor(model, accessor_idx);

  glGenBuffers(1, &buffer_handle);
  glBindBuffer(buffer_view.target, buffer_handle);
  auto buffer_data = std::span{ buffer.data }.subspan(buffer_view.byteOffset,
                                                      buffer_view.byteLength);
  glBufferData(buffer_view.target,
               static_cast<GLsizeiptr>(buffer_data.size()),
               buffer_data.data(),
               GL_STATIC_DRAW);
}

Mesh::Mesh(tinygltf::Model& model)
{
  try {
    glGenVertexArrays(1, &_vertex_array_object);
    glBindVertexArray(_vertex_array_object);

    auto primitive = model.meshes.at(0).primitives.at(0);
    this->_mode = primitive.mode;

    auto accessor = model.accessors.at(primitive.indices);
    this->_count = accessor.count;
    this->_componentType = accessor.componentType;

    auto vbo_accessor_id = primitive.attributes.at("POSITION");
    create_buffer_from_gltf(_vertex_buffer_object, model, vbo_accessor_id);

    glVertexAttribPointer(
      VERTEX_LOCATION, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(VERTEX_LOCATION);

    auto nbo_accessor_id = primitive.attributes.at("NORMAL");
    create_buffer_from_gltf(_normal_buffer_object, model, nbo_accessor_id);

    glVertexAttribPointer(
      NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(NORMAL_LOCATION);

    create_buffer_from_gltf(_element_buffer_object, model, primitive.indices);

    if (has_key(primitive.attributes, TEXT_COORD_NAME)) {
      auto tex_0_accessor_id = primitive.attributes.at(TEXT_COORD_NAME);
      create_buffer_from_gltf(
        _texcoord_buffer_object, model, tex_0_accessor_id);
    } else {
      auto [bufferView, _] = extract_data_by_accessor(model, vbo_accessor_id);
      auto triangle_quantity = bufferView.byteLength / (sizeof(float) * 3) / 3;
      std::vector<float> data;
      for (unsigned int i = 0; i < triangle_quantity; i++) {
        data.emplace_back(0.0f);
        data.emplace_back(0.0f);
        data.emplace_back(0.0f);
        data.emplace_back(1.0f);
        data.emplace_back(1.0f);
        data.emplace_back(1.0f);
      }
      glGenBuffers(1, &_texcoord_buffer_object);
      glBindBuffer(GL_ARRAY_BUFFER, _texcoord_buffer_object);
      glBufferData(GL_ARRAY_BUFFER,
                   static_cast<GLsizeiptr>(data.size() * sizeof(float)),
                   data.data(),
                   GL_STATIC_DRAW);
    }

    glVertexAttribPointer(
      TEXCOORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(TEXCOORD_LOCATION);
  } catch (...) {
    free();
    throw;
  }
}

Mesh::Mesh(std::vector<float>& vertices,
           std::vector<float>& normals,
           std::vector<float>& tex_coords,
           std::vector<unsigned int>& indices)
  : _count(indices.size())
{
  glGenVertexArrays(1, &_vertex_array_object);
  glBindVertexArray(_vertex_array_object);

  create_buffer(_vertex_buffer_object, GL_ARRAY_BUFFER, vertices);
  glVertexAttribPointer(
    VERTEX_LOCATION, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(VERTEX_LOCATION);

  create_buffer(_normal_buffer_object, GL_ARRAY_BUFFER, normals);
  glVertexAttribPointer(
    NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(NORMAL_LOCATION);

  create_buffer(_texcoord_buffer_object, GL_ARRAY_BUFFER, tex_coords);
  glVertexAttribPointer(
    TEXCOORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
  glEnableVertexAttribArray(TEXCOORD_LOCATION);

  create_buffer(_element_buffer_object, GL_ELEMENT_ARRAY_BUFFER, indices);
}

void
Mesh::free()
{
  glBindVertexArray(_vertex_array_object);
  glDeleteBuffers(1, &_vertex_buffer_object);
  glDeleteBuffers(1, &_element_buffer_object);
  glDeleteBuffers(1, &_normal_buffer_object);
  glDeleteBuffers(1, &_texcoord_buffer_object);
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &_vertex_array_object);
}

Mesh::~Mesh()
{
  free();
}

void
Mesh::draw()
{
  glBindVertexArray(_vertex_array_object);
  glDrawElements(_mode, static_cast<GLsizei>(_count), _componentType, nullptr);
  glBindVertexArray(0);
}

std::unique_ptr<Mesh>
Mesh::quad(float z)
{
  std::vector<float> vertices = {
    -1.0, 1.0, z, 1.0, 1.0, z, -1.0, -1.0, z, 1.0, -1.0, z,
  };
  std::vector<float> normals = {
    0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
  };
  std::vector<float> tex_coords = {
    0.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 0.0,
  };
  std::vector<unsigned int> indices = { 2, 1, 0, 2, 3, 1 };
  return std::make_unique<Mesh>(vertices, normals, tex_coords, indices);
}
