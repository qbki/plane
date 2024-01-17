#include <GL/glew.h>
#include <stdexcept>
#include <string>

#include "shader.h"

void
throw_on_shader_error(GLuint shader, GLenum status)
{
  GLint result = GL_FALSE;
  glGetShaderiv(shader, status, &result);
  if (result == GL_FALSE) {
    GLint log_length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
    std::string log_text;
    log_text.resize(log_length);
    glGetShaderInfoLog(shader, log_length, nullptr, log_text.data());
    throw std::runtime_error(log_text);
  }
}

void
throw_on_program_error(GLuint program, GLenum status)
{
  GLint result = GL_FALSE;
  glGetProgramiv(program, status, &result);
  if (result == GL_FALSE) {
    GLint log_length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
    std::string log_text;
    log_text.resize(log_length);
    glGetProgramInfoLog(program, log_length, nullptr, log_text.data());
    throw std::runtime_error(log_text);
  }
}

Shader::~Shader()
{
  glDeleteProgram(_program);
}

void
Shader::create_program()
{
  _program = glCreateProgram();
  if (_program == 0) {
    throw std::runtime_error("Can't create a program.");
  }
}

GLuint
Shader::compile_shader(std::string& text, unsigned int shader_type)
{
  auto shader = glCreateShader(shader_type);
  if (shader == 0) {
    throw std::runtime_error("Can't create a shader.");
  }

  auto shader_text = text.c_str();
  glShaderSource(shader, 1, &shader_text, nullptr);
  glCompileShader(shader);

  throw_on_shader_error(shader, GL_COMPILE_STATUS);

  glAttachShader(_program, shader);

  return shader;
}

void
Shader::link()
{
  glLinkProgram(_program);
  throw_on_program_error(_program, GL_LINK_STATUS);
}

void
Shader::use()
{
  glUseProgram(_program);
}

void
Shader::compile(std::string& vertext, std::string& fragment)
{
  create_program();
  auto vertex_shader = compile_shader(vertext, GL_VERTEX_SHADER);
  auto fragment_shader = compile_shader(fragment, GL_FRAGMENT_SHADER);
  link();
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}

void
Shader::uniform(const std::string& name, const int value)
{
  auto location = glGetUniformLocation(_program, name.c_str());
  glUniform1i(location, value);
}

void
Shader::uniform(const std::string& name, const float value)
{
  auto location = glGetUniformLocation(_program, name.c_str());
  glUniform1f(location, value);
}

void
Shader::uniform(const std::string& name, const glm::mat3& value)
{
  auto location = glGetUniformLocation(_program, name.c_str());
  glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
}

void
Shader::uniform(const std::string& name, const glm::mat4& value)
{
  auto location = glGetUniformLocation(_program, name.c_str());
  glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

void
Shader::uniform(const std::string& name, const glm::vec2& value)
{
  auto location = glGetUniformLocation(_program, name.c_str());
  glUniform2f(location, value.x, value.y);
}

void
Shader::uniform(const std::string& name, const glm::vec3& value)
{
  auto location = glGetUniformLocation(_program, name.c_str());
  glUniform3f(location, value.x, value.y, value.z);
}

void
Shader::uniform(const std::string& name, const glm::vec4& value)
{
  auto location = glGetUniformLocation(_program, name.c_str());
  glUniform4f(location, value.x, value.y, value.z, value.w);
}
