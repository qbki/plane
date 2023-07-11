#pragma once
#include <string>
#include <GL/gl.h>

class Shader {
private:
  GLuint program = 0;

public:
  Shader();
  ~Shader();
  GLuint compile(std::string& text, GLuint shader_type);
  void link();
  void use();
  void createProgram();
  void load(std::string& vertext, std::string& fragment);
};
