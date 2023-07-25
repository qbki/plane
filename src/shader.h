#pragma once
#include <string>
#include <GL/gl.h>
#include <glm/matrix.hpp>

class Shader {
private:
  GLuint program = 0;
  GLuint compile_shader(std::string& text, GLuint shader_type);
  void createProgram();
  void link();

public:
  Shader();
  ~Shader();
  void use();
  void compile(std::string& vertext, std::string& fragment);

  void setUniform(const std::string &name, const int value);
  void setUniform(const std::string &name, const float value);
  void setUniform(const std::string &name, const glm::mat3 &value);
  void setUniform(const std::string &name, const glm::mat4 &value);
  void setUniform(const std::string &name, const glm::vec2 &value);
  void setUniform(const std::string &name, const glm::vec3 &value);
  void setUniform(const std::string &name, const glm::vec4 &value);
};
