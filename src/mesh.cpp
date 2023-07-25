#include <GL/glew.h>

#include "./mesh.h"

const int VERTEX_LOCATION = 0;
const int NORMAL_LOCATION = 1;


void create_buffer(GLuint &buffer_handle, const tinygltf::Model &model, int accessor_idx) {
  auto accessor = model.accessors.at(accessor_idx);
  auto bufferView = model.bufferViews.at(accessor.bufferView);
  auto buffer = model.buffers.at(bufferView.buffer);

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

    // I prepare models myself, so I don't expect a sophisticated structure.
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
}


Mesh::~Mesh() {
  free();
}


void Mesh::draw() {
  glBindVertexArray(vao);
  glDrawElements(
    mode,
    count,
    componentType,
    0
  );
  glBindVertexArray(0);
}
