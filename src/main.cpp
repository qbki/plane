#include <atomic>
#include <glm/common.hpp>
#include <iterator>
#define GLM_ENABLE_EXPERIMENTAL
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION
#include <iostream>
#include <vector>
#include <memory>
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <glm/vec2.hpp>
#include <glm/gtx/string_cast.hpp>
#include <tiny_gltf.h>

#include "shader.h"

GLuint POSITION_LOCATION = 0;

std::string VERTEX_SHADER = R"""(
  #version 330 core
  layout (location = 0) in vec3 aPos;

  void main()
  {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
  }
)""";

std::string FRAGMENT_SHADER = R"""(
  #version 330 core
  out vec4 FragColor;

  void main()
  {
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
  }
)""";

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

template<typename T>
unsigned int byte_size_of_vector(std::vector<T> array) {
  return sizeof(T) * array.size();
}

int buffer_size() {
  GLint size = 0;
  glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
  return size;
}

template<typename T>
void print(T& value) {
  std::cout << value << std::endl;
}

tinygltf::Model loadModel(const std::string& filename) {
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

int main() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    auto err_text = std::string("Unable to init SDL: ") + SDL_GetError();
    throw new std::runtime_error(err_text);
  }
  std::cout << "SDL has been initialized." << std::endl;

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  auto window = SDL_CreateWindow(
    "Plane",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    800, 600,
    SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if (window == nullptr) {
    auto err_sdl = std::unique_ptr<const char>(SDL_GetError());
    auto err_text = std::string("Unable to create window: ") + *err_sdl;
    throw new std::runtime_error(err_text);
  }
  std::cout << "Window has been created." << std::endl;

  auto ctx = SDL_GL_CreateContext(window);
  if (ctx == nullptr) {
    auto err_sdl = std::unique_ptr<const char>(SDL_GetError());
    auto err_text = std::string("Unable to create GL Context: ") + *err_sdl;
    throw new std::runtime_error(err_text);
  }
  std::cout << "Context has been created." << std::endl;

  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    auto err_glew = std::unique_ptr<const char>(reinterpret_cast<const char*>(glewGetErrorString(err)));
    auto err_text = std::string() + *err_glew;
    throw new std::runtime_error(err_text);
  }
  std::cout << "GLEW has been inited." << std::endl;

  glEnable(GL_DEPTH_TEST);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  SDL_SetRelativeMouseMode(SDL_TRUE);

  bool is_running = true;

  Shader shader;
  shader.load(VERTEX_SHADER, FRAGMENT_SHADER);

  GLuint vao; // vertex array object
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  auto model = loadModel("./models/plane.glb");
  auto primitive = model.meshes[0].primitives[0];

  {
    auto value = primitive.attributes.at("POSITION");
    auto accessor = model.accessors.at(value);
    auto bufferView = model.bufferViews.at(accessor.bufferView);
    auto buffer = model.buffers.at(bufferView.buffer);

    GLuint vbo; // vertex buffer object
    glGenBuffers(1, &vbo);
    glBindBuffer(bufferView.target, vbo);
    glBufferData(
      bufferView.target,
      bufferView.byteLength,
      buffer.data.data(),
      GL_STATIC_DRAW
    );
  }

  {
    auto accessor = model.accessors.at(primitive.indices);
    auto bufferView = model.bufferViews.at(accessor.bufferView);
    auto buffer = model.buffers.at(bufferView.buffer);

    GLuint ebo; // element buffer object
    glGenBuffers(1, &ebo);
    glBindBuffer(bufferView.target, ebo);
    glBufferData(
      bufferView.target,
      bufferView.byteLength,
      &buffer.data.at(0) + bufferView.byteOffset,
      GL_STATIC_DRAW
    );
  }

  auto accessor = model.accessors[primitive.indices];
  auto stride = accessor.ByteStride(model.bufferViews.at(accessor.bufferView));

  glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
  glEnableVertexAttribArray(POSITION_LOCATION);

  SDL_Event event;
  while (is_running) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT: {
          is_running = false;
          break;
        }
      }
    }

    shader.use();
    glBindVertexArray(vao);
    glDrawElements(
      primitive.mode,
      accessor.count,
      accessor.componentType,
      0
    );
    glBindVertexArray(0);
    SDL_GL_SwapWindow(window);
  }

  SDL_GL_DeleteContext(ctx);
  SDL_DestroyWindow(window);
}
