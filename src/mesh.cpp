#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL_rwops.h>
#include <exception>
#include <iostream>
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


void create_buffer(GLuint &buffer_handle, const tinygltf::Model &model, int accessor_idx) {
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
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    auto primitive = model.meshes.at(0).primitives.at(0);
    this->mode = primitive.mode;

    auto accessor = model.accessors.at(primitive.indices);
    this->count = accessor.count;
    this->componentType = accessor.componentType;

    auto vbo_accessor_id = primitive.attributes.at("POSITION");
    create_buffer(vbo, model, vbo_accessor_id);

    glVertexAttribPointer(VERTEX_LOCATION, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(VERTEX_LOCATION);

    auto nbo_accessor_id = primitive.attributes.at("NORMAL");
    create_buffer(nbo, model, nbo_accessor_id);

    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(NORMAL_LOCATION);

    create_buffer(ebo, model, primitive.indices);

    if (has_key(primitive.attributes, TEXT_COORD_NAME)) {
      auto tex_0_accessor_id = primitive.attributes.at(TEXT_COORD_NAME);
      create_buffer(tbo, model, tex_0_accessor_id);
    } else {
      auto [bufferView, _] = extract_data_by_accessor(model, vbo_accessor_id);
      auto triangle_quantity = bufferView.byteLength / (sizeof(float) * 3) / 3;
      std::cout << triangle_quantity << std::endl;
      std::vector<float> data;
      // data.reserve(triangle_quantity * 3 * 2);
      for (int i = 0; i < triangle_quantity; i++) {
        data.emplace_back(0.0f);
        data.emplace_back(0.0f);
        data.emplace_back(0.0f);
        data.emplace_back(1.0f);
        data.emplace_back(1.0f);
        data.emplace_back(1.0f);
      }
      glGenBuffers(1, &tbo);
      glBindBuffer(GL_ARRAY_BUFFER, tbo);
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


void Mesh::free() {
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
  glDeleteBuffers(1, &nbo);
  glDeleteBuffers(1, &tbo);
}


Mesh::~Mesh() {
  free();
}


void Mesh::draw() {
  glBindVertexArray(vao);
  glDrawElements(mode, count, componentType, 0);
  glBindVertexArray(0);
}
