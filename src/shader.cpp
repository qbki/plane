#include <GL/glew.h>
#include <stdexcept>
#include <memory>
#include <string>

#include "shader.h"


void throw_on_shader_error(GLuint shader, GLenum status) {
  GLint result;
  glGetShaderiv(shader, status, &result);
  if (result == GL_FALSE) {
    GLint log_length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

    auto log_text = new std::unique_ptr<GLchar[]>(new GLchar[log_length]);
    glGetShaderInfoLog(shader, log_length, nullptr, log_text->get());

    throw std::runtime_error(std::string(log_text->get()));
  }
}


void throw_on_program_error(GLuint program, GLenum status) {
  GLint result;
  glGetProgramiv(program, status, &result);
  if (result == GL_FALSE) {
    GLint log_length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);

    auto log_text = new std::unique_ptr<GLchar[]>(new GLchar[log_length]);
    glGetProgramInfoLog(program, log_length, nullptr, log_text->get());

    throw std::runtime_error(std::string(log_text->get()));
  }
}


Shader::Shader() {}


Shader::~Shader() {
  glDeleteProgram(program);
}


void Shader::create_program() {
  this->program = glCreateProgram();
  if (this->program == 0) {
    throw std::runtime_error("Can't create a program.");
  }
}


GLuint Shader::compile_shader(std::string& text, GLuint shader_type) {
  auto shader = glCreateShader(shader_type);
  if (shader == 0) {
    throw std::runtime_error("Can't create a shader.");
  }

  auto shader_text = text.c_str();
  glShaderSource(shader, 1, &shader_text, NULL);
  glCompileShader(shader);

  throw_on_shader_error(shader, GL_COMPILE_STATUS);

  glAttachShader(program, shader);

  return shader;
}


void Shader::link() {
  glLinkProgram(program);
  throw_on_program_error(program, GL_LINK_STATUS);
}


void Shader::use() {
  glUseProgram(program);
}


void Shader::compile(std::string& vertext, std::string& fragment) {
  create_program();
  auto vertex_shader = compile_shader(vertext, GL_VERTEX_SHADER);
  auto fragment_shader = compile_shader(fragment, GL_FRAGMENT_SHADER);
  link();
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}


void Shader::uniform(const std::string &name, const int value) {
  GLuint location = glGetUniformLocation(program, name.c_str());
  glUniform1i(location, value);
}


void Shader::uniform(const std::string &name, const float value) {
  GLuint location = glGetUniformLocation(program, name.c_str());
  glUniform1f(location, value);
}


void Shader::uniform(const std::string &name, const glm::mat3 &value) {
  GLuint location = glGetUniformLocation(program, name.c_str());
  glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
}


void Shader::uniform(const std::string &name, const glm::mat4 &value) {
  GLuint location = glGetUniformLocation(program, name.c_str());
  glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}


void Shader::uniform(const std::string &name, const glm::vec2 &value) {
  GLuint location = glGetUniformLocation(program, name.c_str());
  glUniform2f(location, value.x, value.y);
}


void Shader::uniform(const std::string &name, const glm::vec3 &value) {
  GLuint location = glGetUniformLocation(program, name.c_str());
  glUniform3f(location, value.x, value.y, value.z);
}


void Shader::uniform(const std::string &name, const glm::vec4 &value) {
  GLuint location = glGetUniformLocation(program, name.c_str());
  glUniform4f(location, value.x, value.y, value.z, value.w);
}
