#pragma once
#include <string>
#include <glm/matrix.hpp>


class Shader {
private:
  unsigned int _program = 0;
  unsigned int compile_shader(std::string& text, unsigned int shader_type);
  void create_program();
  void link();

public:
  Shader();
  ~Shader();
  void use();
  void compile(std::string& vertext, std::string& fragment);

  void uniform(const std::string &name, const int value);
  void uniform(const std::string &name, const float value);
  void uniform(const std::string &name, const glm::mat3 &value);
  void uniform(const std::string &name, const glm::mat4 &value);
  void uniform(const std::string &name, const glm::vec2 &value);
  void uniform(const std::string &name, const glm::vec3 &value);
  void uniform(const std::string &name, const glm::vec4 &value);
};
