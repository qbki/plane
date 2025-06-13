#include <GL/glew.h>
#include <string>

#include "shader.h"

import utils.crash;

void
crash_on_shader_error(GLuint shader, GLenum status)
{
  GLint result = GL_FALSE;
  glGetShaderiv(shader, status, &result);
  if (result == GL_FALSE) {
    GLint log_length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
    std::string log_text;
    log_text.resize(log_length);
    glGetShaderInfoLog(shader, log_length, nullptr, log_text.data());
    utils::crash(log_text);
  }
}

void
crash_on_program_error(GLuint program, GLenum status)
{
  GLint result = GL_FALSE;
  glGetProgramiv(program, status, &result);
  if (result == GL_FALSE) {
    GLint log_length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
    std::string log_text;
    log_text.resize(log_length);
    glGetProgramInfoLog(program, log_length, nullptr, log_text.data());
    utils::crash(log_text);
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
    utils::crash("Can't create a program.");
  }
}

GLuint
Shader::compile_shader(std::string& text, unsigned int shader_type)
{
  auto shader = glCreateShader(shader_type);
  if (shader == 0) {
    utils::crash("Can't create a shader.");
  }

  auto shader_text = text.c_str();
  glShaderSource(shader, 1, &shader_text, nullptr);
  glCompileShader(shader);

  crash_on_shader_error(shader, GL_COMPILE_STATUS);

  glAttachShader(_program, shader);

  return shader;
}

void
Shader::link()
{
  glLinkProgram(_program);
  crash_on_program_error(_program, GL_LINK_STATUS);
}

GLint
Shader::get_location(const std::string& location_name)
{
  if (_cached_locations.contains(location_name)) {
    return _cached_locations[location_name];
  }
  auto location = glGetUniformLocation(_program, location_name.c_str());
  _cached_locations[location_name] = location;
  return location;
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
  glUniform1i(get_location(name), value);
}

void
Shader::uniform(const std::string& name, const float value)
{
  glUniform1f(get_location(name), value);
}

void
Shader::uniform(const std::string& name, const glm::mat3& value)
{
  glUniformMatrix3fv(get_location(name), 1, GL_FALSE, &value[0][0]);
}

void
Shader::uniform(const std::string& name, const glm::mat4& value)
{
  glUniformMatrix4fv(get_location(name), 1, GL_FALSE, &value[0][0]);
}

void
Shader::uniform(const std::string& name, const glm::vec2& value)
{
  glUniform2f(get_location(name), value.x, value.y);
}

void
Shader::uniform(const std::string& name, const glm::vec3& value)
{
  glUniform3f(get_location(name), value.x, value.y, value.z);
}

void
Shader::uniform(const std::string& name, const glm::vec4& value)
{
  glUniform4f(get_location(name), value.x, value.y, value.z, value.w);
}
