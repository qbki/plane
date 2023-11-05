#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/ext/vector_float3.hpp>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <stdexcept>

#include "utils.h"


int buffer_size() {
  GLint size = 0;
  glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
  return size;
}


glm::vec3 calc_z_direction(const glm::vec3& rotation) {
  return glm::vec3(glm::cos(rotation.z), glm::sin(rotation.z), 0.0);
}


glm::vec3 encode_gamma(const glm::vec3& color, float gamma) {
  return glm::pow(color, glm::vec3(1.0 / gamma));
}


glm::vec3 decode_gamma(const glm::vec3& color, float gamma) {
  return glm::pow(color, glm::vec3(gamma));
}


tinygltf::Model load_gltf_model(const std::string& filename) {
  tinygltf::Model model;
  tinygltf::TinyGLTF loader;
  std::string err;
  std::string warn;

  if (!std::filesystem::exists(filename)) {
    throw std::runtime_error("File not found: " + filename);
  }

  bool res = loader.LoadBinaryFromFile(&model, &err, &warn, filename);
  if (!warn.empty()) {
    std::cout << "WARN: " << warn << std::endl;
  }

  if (!err.empty()) {
    std::cout << "ERR: " << err << std::endl;
  }

  if (!res) {
    throw std::runtime_error("Failed to load glTF: " + filename);
  } else {
    std::cout << "Loaded glTF: " << filename << std::endl;
  }

  return model;
}


std::string load_text(const std::string &file_name) {
  std::ifstream ifs(file_name);

  if (ifs.fail()) {
    throw std::runtime_error("Cannot read a file: " + file_name);
  }

  std::string content(
    (std::istreambuf_iterator<char>(ifs)),
    (std::istreambuf_iterator<char>())
  );
  return content;
}


void resize_window(
  const SDL_WindowEvent& window_event,
  Camera& camera
) {
  auto width = window_event.data1;
  auto height = window_event.data2;
  camera.aspect_ratio(static_cast<float>(width) / height);
  glViewport(0, 0, width, height);
}


glm::mat4 make_transform_matrix(glm::vec3 position, glm::vec3 rotation) {
  auto rotation_matrix = glm::rotate(glm::mat4(1.0), rotation.z, {0.0, 0.0, 1.0});
  auto transform_matrix = glm::translate(glm::mat4(1.0), position);
  return transform_matrix * rotation_matrix;
}
