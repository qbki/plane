#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include <fstream>
#include <stdexcept>

#include "./utils.h"


int buffer_size() {
  GLint size = 0;
  glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
  return size;
}


template<typename T>
void print(T& value) {
  std::cout << value << std::endl;
}


tinygltf::Model load_gltf_model(const std::string& filename) {
  tinygltf::Model model;
  tinygltf::TinyGLTF loader;
  std::string err;
  std::string warn;

  bool res = loader.LoadBinaryFromFile(&model, &err, &warn, filename);
  if (!warn.empty()) {
    std::cout << "WARN: " << warn << std::endl;
  }

  if (!err.empty()) {
    std::cout << "ERR: " << err << std::endl;
  }

  if (!res) {
    throw new std::runtime_error("Failed to load glTF: " + filename);
  } else {
    std::cout << "Loaded glTF: " << filename << std::endl;
  }

  return model;
}


std::string load_text(const std::string &file_name) {
  std::ifstream ifs(file_name);

  if (ifs.fail()) {
    throw new std::runtime_error("Cannot read a file: " + file_name);
  }

  std::string content(
    (std::istreambuf_iterator<char>(ifs)),
    (std::istreambuf_iterator<char>())
  );
  return content;
}