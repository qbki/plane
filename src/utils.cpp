#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/ext/vector_float3.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
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
    auto message = "Cannot read a file: " + file_name;
    throw std::runtime_error(message);
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

/**
 * Thanks to the documentation: https://www.khronos.org/opengl/wiki/OpenGL_Error
 */
void print_opengl_errors() {
  GLenum error;
  while ((error = glGetError()) != GL_NO_ERROR) {
    std::stringstream ss;
    ss << "Error: ";
    switch (error) {
      case GL_INVALID_ENUM: {
        ss << "An enumeration parameter is not legal (GL_INVALID_ENUM)";
        break;
      }
      case GL_INVALID_VALUE: {
        ss << "A value parameter is not a legal value (GL_INVALID_VALUE)";
        break;
      }
      case GL_INVALID_OPERATION: {
        ss << "The set of state for a command is not legal (GL_INVALID_OPERATION)";
        break;
      }
      case GL_STACK_OVERFLOW: {
        ss << "A stack pushing operation cannot be done because it would overflow the limit of that stack's size (GL_STACK_OVERFLOW)";
        break;
      }
      case GL_STACK_UNDERFLOW: {
        ss << "A stack popping operation cannot be done because the stack is already at its lowest point (GL_STACK_UNDERFLOW)";
        break;
      }
      case GL_OUT_OF_MEMORY: {
        ss << "The memory cannot be allocated (GL_OUT_OF_MEMORY)";
        break;
      }
      case GL_INVALID_FRAMEBUFFER_OPERATION: {
        ss << "Reading from or writeing/rendering to a framebuffer that is not complete (GL_INVALID_FRAMEBUFFER_OPERATION)";
        break;
      }
      case GL_CONTEXT_LOST: {
        ss << "The OpenGL context has been lost (GL_CONTEXT_LOST)";
        break;
      }
      default: {
        ss << "Unknown error: " << error;
        break;
      }
    }
    print_error(ss.str());
  }
}


void print_opengl_info() {
  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl
    << "Vendor: " << glGetString(GL_VENDOR) << std::endl
    << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
}


void print_extension_support(std::string extension_name) {
  int quantity = 0;
  glGetIntegerv(GL_NUM_EXTENSIONS, &quantity);
  for (int i = 0; i < quantity; i++)
  {
    std::string extension(reinterpret_cast<const char *>(glGetStringi(GL_EXTENSIONS, i)));
    if (extension_name == extension) {
      print(extension_name + ": is supported");
      return;
    }
  }
  print(extension_name + ": is not supported");
  return;
}
