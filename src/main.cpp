#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <vector>
#include <memory>
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <glm/vec2.hpp>
#include <glm/gtx/string_cast.hpp>

#include "./shader.h"
#include "./mesh.h"
#include "./utils.h"

GLuint POSITION_LOCATION = 0;

std::string VERTEX_SHADER = R"""(
  #version 330 core
  layout (location = 0) in vec3 aPos;

  void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
  }
)""";

std::string FRAGMENT_SHADER = R"""(
  #version 330 core
  out vec4 FragColor;

  void main() {
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
  }
)""";

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

  auto model = loadModel("./models/plane.glb");
  Mesh mesh(model);

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
    mesh.draw();
    SDL_GL_SwapWindow(window);
  }

  SDL_GL_DeleteContext(ctx);
  SDL_DestroyWindow(window);
}
