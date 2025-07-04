module;
#include <GL/glew.h>
#include <algorithm>
#include <cstddef>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <map>
#include <span>
#include <string>
#include <tiny_gltf.h>
#include <tuple>
#include <vector>

export module pln.mesh;

import pln.math.intersections;
import pln.math.shapes;
import pln.utils.gl;

using namespace pln::math;
using namespace pln::utils;

namespace pln::mesh {

export class Mesh
{
private:
  constexpr static auto TEXT_COORD_NAME = "TEXCOORD_0";
  constexpr static float GL_BUFFER_MULTIPLAYER = 2.0;
  constexpr static int VERTEX_LOCATION {0};
  constexpr static int NORMAL_LOCATION {1};
  constexpr static int TEXCOORD_LOCATION {2};
  constexpr static int MODEL_TRANSFORM_1_LOCATION {3};
  constexpr static int MODEL_TRANSFORM_2_LOCATION {4};
  constexpr static int MODEL_TRANSFORM_3_LOCATION {5};
  constexpr static int MODEL_TRANSFORM_4_LOCATION {6};

  static auto
  extract_data_by_accessor(const tinygltf::Model& model, int accessor_idx)
  {
    auto accessor = model.accessors.at(accessor_idx);
    auto bufferView = model.bufferViews.at(accessor.bufferView);
    auto buffer = model.buffers.at(bufferView.buffer);
    return std::make_tuple(bufferView, buffer);
  }

public:
  struct DrawParams
  {
    std::vector<glm::mat4> transforms;
  };

private:
  GLuint _vertex_array_object {0};
  GLuint _vertex_buffer_object {0};
  GLuint _normal_buffer_object {0};
  GLuint _transform_buffer_object {0};
  GLuint _element_buffer_object {0};
  GLuint _texcoord_buffer_object {0};

  int _mode {GL_TRIANGLES};
  size_t _number_of_elements {0};
  size_t _instance_quantity {1};
  int _componentType {GL_UNSIGNED_INT};
  pln::math::Collider _bounding_volume;



public:
  Mesh(const Mesh&) = delete;
  Mesh& operator=(const Mesh&) = delete;
  Mesh(Mesh&&) = delete;
  Mesh& operator=(Mesh&&) = delete;


  explicit Mesh(tinygltf::Model& model)
  {
    calculate_bounding_volume(model);

    glGenVertexArrays(1, &_vertex_array_object);
    glBindVertexArray(_vertex_array_object);

    auto primitive = model.meshes.at(0).primitives.at(0);
    this->_mode = primitive.mode;

    auto accessor = model.accessors.at(primitive.indices);
    this->_number_of_elements = accessor.count;
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

    if (primitive.attributes.contains(TEXT_COORD_NAME)) {
      auto tex_0_accessor_id = primitive.attributes.at(TEXT_COORD_NAME);
      create_buffer_from_gltf(_texcoord_buffer_object, model, tex_0_accessor_id);
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
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    glVertexAttribPointer(
      TEXCOORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(TEXCOORD_LOCATION);

    print_opengl_errors("Mesh::Mesh");

    create_transform_attribute(_transform_buffer_object, _instance_quantity);
  }


  Mesh(std::vector<float>& vertices,
       std::vector<float>& normals,
       std::vector<float>& tex_coords,
       std::vector<unsigned int>& indices)
    : _number_of_elements(indices.size())
  {
    pln::math::Sphere volume = {
      .center = { 0, 0, 0 },
      .radius = 0,
    };
    for (auto& vertex : vertices) {
      volume.radius = glm::max(glm::length(vertex), volume.radius);
    }
    _bounding_volume = volume;

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

    create_transform_attribute(_transform_buffer_object, _instance_quantity);
  }


  ~Mesh()
  {
    free();
  }


  void draw(const DrawParams& draw_params)
  {
    auto& transforms = draw_params.transforms;
    if (transforms.size() > _instance_quantity) {
      _instance_quantity = resize_buffer_data(
          _transform_buffer_object, GL_ARRAY_BUFFER, transforms, glm::mat4(1.0));
    } else {
      populate_buffer(_transform_buffer_object, transforms);
    }
    glBindVertexArray(_vertex_array_object);
    glDrawElementsInstanced(_mode,
                            static_cast<GLsizei>(_number_of_elements),
                            _componentType,
                            nullptr,
                            static_cast<GLsizei>(transforms.size()));
    glBindVertexArray(0);
  }


  [[nodiscard]] const pln::math::Collider& bounding_volume() const
  {
    return _bounding_volume;
  }

private:
  void free()
  {
    glBindVertexArray(_vertex_array_object);
    glDeleteBuffers(1, &_vertex_buffer_object);
    glDeleteBuffers(1, &_element_buffer_object);
    glDeleteBuffers(1, &_normal_buffer_object);
    glDeleteBuffers(1, &_transform_buffer_object);
    glDeleteBuffers(1, &_texcoord_buffer_object);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &_vertex_array_object);
  }


  void calculate_bounding_volume(tinygltf::Model& model)
  {
    pln::math::Sphere volume {
      .center = { 0, 0, 0 },
      .radius = 0,
    };
    auto primitive = model.meshes.at(0).primitives.at(0);
    auto vbo_accessor_id = primitive.attributes.at("POSITION");
    auto accessor = model.accessors.at(vbo_accessor_id);
    auto [buffer_view, buffer] = extract_data_by_accessor(model, vbo_accessor_id);
    auto offset = buffer_view.byteOffset + accessor.byteOffset;
    // see: https://github.com/syoyo/tinygltf/wiki/Accessing-vertex-data
    // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
    auto data = reinterpret_cast<const float*>(&buffer.data[offset]); //
    for (size_t i = 0; i < accessor.count; i++) {
      // NOLINTNEXTLINE (cppcoreguidelines-pro-bounds-pointer-arithmetic)
      glm::vec3 vertex = { data[i * 3 + 0], data[i * 3 + 1], data[i * 3 + 2] };
      volume.radius = glm::max(volume.radius, glm::length(vertex));
    }
    _bounding_volume = volume;
  }


  template<typename T>
  static void
  replace_buffer_data(GLuint buffer_handle,
                      GLuint target,
                      std::vector<T>& data,
                      GLenum usage = GL_STATIC_DRAW)
  {
    glBindBuffer(target, buffer_handle);
    glBufferData(target, data.size() * sizeof(T), data.data(), usage);
  }


  template<typename T>
  static size_t
  resize_buffer_data(GLuint buffer_handle,
                     GLuint target,
                     const std::vector<T>& data,
                     const T& default_item)
  {
    auto new_size = static_cast<size_t>(static_cast<float>(data.size())
                                        * GL_BUFFER_MULTIPLAYER);
    std::vector<T> new_data(new_size, default_item);
    std::ranges::copy(data, begin(new_data));
    replace_buffer_data(buffer_handle, target, new_data, GL_DYNAMIC_DRAW);
    return new_size;
  }


  template<typename T>
  static void
  create_buffer(GLuint& buffer_handle,
                GLuint target,
                std::vector<T>& data,
                GLenum usage = GL_STATIC_DRAW)
  {
    glGenBuffers(1, &buffer_handle);
    replace_buffer_data(buffer_handle, target, data, usage);
  }


  static void
  create_buffer_from_gltf(GLuint& buffer_handle,
                          const tinygltf::Model& model,
                          int accessor_idx)
  {
    auto [buffer_view, buffer] = extract_data_by_accessor(model, accessor_idx);

    glGenBuffers(1, &buffer_handle);
    glBindBuffer(buffer_view.target, buffer_handle);
    auto buffer_data = std::span { buffer.data }.subspan(buffer_view.byteOffset,
                                                         buffer_view.byteLength);
    glBufferData(buffer_view.target,
                 static_cast<GLsizeiptr>(buffer_data.size()),
                 buffer_data.data(),
                 GL_STATIC_DRAW);
  }


  void
  create_transform_attribute(GLuint& buffer_object, size_t instance_quantity)
  {
    std::vector<glm::mat4> identity_matrices(instance_quantity, glm::mat4(1.0));
    create_buffer(
      buffer_object, GL_ARRAY_BUFFER, identity_matrices, GL_DYNAMIC_DRAW);
    const auto stride = sizeof(glm::mat4);
    // NOLINTBEGIN (cppcoreguidelines-pro-type-reinterpret-cast)
    const auto second_offset = reinterpret_cast<void*>(sizeof(glm::vec4));
    const auto third_offset = reinterpret_cast<void*>(sizeof(glm::vec4) * 2);
    const auto fourth_offset = reinterpret_cast<void*>(sizeof(glm::vec4) * 3);
    // NOLINTEND (cppcoreguidelines-pro-type-reinterpret-cast)
    glVertexAttribPointer(
      MODEL_TRANSFORM_1_LOCATION, 4, GL_FLOAT, GL_FALSE, stride, nullptr);
    glVertexAttribPointer(
      MODEL_TRANSFORM_2_LOCATION, 4, GL_FLOAT, GL_FALSE, stride, second_offset);
    glVertexAttribPointer(
      MODEL_TRANSFORM_3_LOCATION, 4, GL_FLOAT, GL_FALSE, stride, third_offset);
    glVertexAttribPointer(
      MODEL_TRANSFORM_4_LOCATION, 4, GL_FLOAT, GL_FALSE, stride, fourth_offset);
    glVertexAttribDivisor(MODEL_TRANSFORM_1_LOCATION, 1);
    glVertexAttribDivisor(MODEL_TRANSFORM_2_LOCATION, 1);
    glVertexAttribDivisor(MODEL_TRANSFORM_3_LOCATION, 1);
    glVertexAttribDivisor(MODEL_TRANSFORM_4_LOCATION, 1);
    glEnableVertexAttribArray(MODEL_TRANSFORM_1_LOCATION);
    glEnableVertexAttribArray(MODEL_TRANSFORM_2_LOCATION);
    glEnableVertexAttribArray(MODEL_TRANSFORM_3_LOCATION);
    glEnableVertexAttribArray(MODEL_TRANSFORM_4_LOCATION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    print_opengl_errors("create_transform_attribute");
  }


  template<typename T>
  static void
  populate_buffer(GLuint buffer_object, const std::vector<T>& data)
  {
    auto size = sizeof(T) * data.size();
    glBindBuffer(GL_ARRAY_BUFFER, buffer_object);
    glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizei>(size), data.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
};

}
