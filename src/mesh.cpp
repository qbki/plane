#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL_rwops.h>
#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <tiny_gltf.h>
#include <tuple>

#include "mesh.h"
#include "utils.h"

const int VERTEX_LOCATION = 0;
const int NORMAL_LOCATION = 1;
const int TEXCOORD_LOCATION = 2;

const auto TEXT_COORD_NAME = "TEXCOORD_0";


auto extract_data_by_accessor(const tinygltf::Model &model, int accessor_idx) {
  auto accessor = model.accessors.at(accessor_idx);
  auto bufferView = model.bufferViews.at(accessor.bufferView);
  auto buffer = model.buffers.at(bufferView.buffer);
  return std::make_tuple(bufferView, buffer);
}


template<typename T>
void create_buffer(GLuint& buffer_handle, GLuint target, std::vector<T>& data) {
  glGenBuffers(1, &buffer_handle);
  glBindBuffer(target, buffer_handle);
  glBufferData(
    target,
    data.size() * sizeof(T),
    data.data(),
    GL_STATIC_DRAW
  );
}


void create_buffer_from_gltf(GLuint &buffer_handle, const tinygltf::Model &model, int accessor_idx) {
  auto [bufferView, buffer] = extract_data_by_accessor(model, accessor_idx);

  glGenBuffers(1, &buffer_handle);
  glBindBuffer(bufferView.target, buffer_handle);
  glBufferData(
    bufferView.target,
    bufferView.byteLength,
    &buffer.data.at(0) + bufferView.byteOffset,
    GL_STATIC_DRAW
  );
}


Mesh::Mesh(tinygltf::Model &model) {
  try {
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    auto primitive = model.meshes.at(0).primitives.at(0);
    this->_mode = primitive.mode;

    auto accessor = model.accessors.at(primitive.indices);
    this->_count = accessor.count;
    this->_componentType = accessor.componentType;

    auto vbo_accessor_id = primitive.attributes.at("POSITION");
    create_buffer_from_gltf(_vbo, model, vbo_accessor_id);

    glVertexAttribPointer(VERTEX_LOCATION, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(VERTEX_LOCATION);

    auto nbo_accessor_id = primitive.attributes.at("NORMAL");
    create_buffer_from_gltf(_nbo, model, nbo_accessor_id);

    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(NORMAL_LOCATION);

    create_buffer_from_gltf(_ebo, model, primitive.indices);

    if (has_key(primitive.attributes, TEXT_COORD_NAME)) {
      auto tex_0_accessor_id = primitive.attributes.at(TEXT_COORD_NAME);
      create_buffer_from_gltf(_tbo, model, tex_0_accessor_id);
    } else {
      auto [bufferView, _] = extract_data_by_accessor(model, vbo_accessor_id);
      auto triangle_quantity = bufferView.byteLength / (sizeof(float) * 3) / 3;
      std::vector<float> data;
      for (int i = 0; i < triangle_quantity; i++) {
        data.emplace_back(0.0f);
        data.emplace_back(0.0f);
        data.emplace_back(0.0f);
        data.emplace_back(1.0f);
        data.emplace_back(1.0f);
        data.emplace_back(1.0f);
      }
      glGenBuffers(1, &_tbo);
      glBindBuffer(GL_ARRAY_BUFFER, _tbo);
      glBufferData(
        GL_ARRAY_BUFFER,
        data.size() * sizeof(float),
        data.data(),
        GL_STATIC_DRAW
      );
    }

    glVertexAttribPointer(TEXCOORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(TEXCOORD_LOCATION);
  } catch (...) {
    free();
    throw;
  }
}


Mesh::Mesh(
  std::vector<float>& vertices,
  std::vector<float>& normals,
  std::vector<float>& tex_coords,
  std::vector<unsigned int>& indices
) {
  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);

  _count = indices.size();

  create_buffer(_vbo, GL_ARRAY_BUFFER, vertices);
  glVertexAttribPointer(VERTEX_LOCATION, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
  glEnableVertexAttribArray(VERTEX_LOCATION);

  create_buffer(_nbo, GL_ARRAY_BUFFER, normals);
  glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
  glEnableVertexAttribArray(NORMAL_LOCATION);

  create_buffer(_tbo, GL_ARRAY_BUFFER, tex_coords);
  glVertexAttribPointer(TEXCOORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *) 0);
  glEnableVertexAttribArray(TEXCOORD_LOCATION);

  create_buffer(_ebo, GL_ELEMENT_ARRAY_BUFFER, indices);
}


void Mesh::free() {
  glBindVertexArray(_vao);
  glDeleteBuffers(1, &_vbo);
  glDeleteBuffers(1, &_ebo);
  glDeleteBuffers(1, &_nbo);
  glDeleteBuffers(1, &_tbo);
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &_vao);
}


Mesh::~Mesh() {
  free();
}


void Mesh::draw() {
  glBindVertexArray(_vao);
  glDrawElements(_mode, _count, _componentType, 0);
  glBindVertexArray(0);
}


std::unique_ptr<Mesh> Mesh::quad(float z) {
  std::vector<float> vertices = {
    -1.0,  1.0, z, 1.0,  1.0, z,
    -1.0, -1.0, z, 1.0, -1.0, z,
  };
  std::vector<float> normals = {
    0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
    0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
  };
  std::vector<float> tex_coords = {
    0.0, 1.0, 1.0, 1.0,
    0.0, 0.0, 1.0, 0.0,
  };
  std::vector<unsigned int> indices = {
    2, 1, 0,
    2, 3, 1
  };
  return std::make_unique<Mesh>(vertices, normals, tex_coords, indices);
}
