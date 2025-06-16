module;
#include <GL/glew.h>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <string>
#include <unordered_map>

export module pln.shaders;

import pln.utils.crash;

namespace pln::shaders {


export class Shader
{
private:
  GLuint _program = 0;
  std::unordered_map<std::string, GLint> _cached_locations;


public:
  Shader() = default;
  Shader(const Shader&) = delete;
  Shader& operator=(const Shader&) = delete;
  Shader(Shader&&) = delete;
  Shader& operator=(Shader&&) = delete;


  ~Shader()
  {
    glDeleteProgram(_program);
  }


  void
  link()
  {
    glLinkProgram(_program);
    crash_on_program_error(_program, GL_LINK_STATUS);
  }


  GLint
  get_location(const std::string& location_name)
  {
    if (_cached_locations.contains(location_name)) {
      return _cached_locations[location_name];
    }
    auto location = glGetUniformLocation(_program, location_name.c_str());
    _cached_locations[location_name] = location;
    return location;
  }


  void
  use()
  {
    glUseProgram(_program);
  }


  void
  compile(std::string& vertext, std::string& fragment)
  {
    create_program();
    auto vertex_shader = compile_shader(vertext, GL_VERTEX_SHADER);
    auto fragment_shader = compile_shader(fragment, GL_FRAGMENT_SHADER);
    link();
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
  }


  void
  uniform(const std::string& name, const int value)
  {
    glUniform1i(get_location(name), value);
  }


  void
  uniform(const std::string& name, const float value)
  {
    glUniform1f(get_location(name), value);
  }


  void
  uniform(const std::string& name, const glm::mat3& value)
  {
    glUniformMatrix3fv(get_location(name), 1, GL_FALSE, &value[0][0]);
  }


  void
  uniform(const std::string& name, const glm::mat4& value)
  {
    glUniformMatrix4fv(get_location(name), 1, GL_FALSE, &value[0][0]);
  }


  void
  uniform(const std::string& name, const glm::vec2& value)
  {
    glUniform2f(get_location(name), value.x, value.y);
  }


  void
  uniform(const std::string& name, const glm::vec3& value)
  {
    glUniform3f(get_location(name), value.x, value.y, value.z);
  }


  void
  uniform(const std::string& name, const glm::vec4& value)
  {
    glUniform4f(get_location(name), value.x, value.y, value.z, value.w);
  }


private:
  void
  create_program()
  {
    _program = glCreateProgram();
    if (_program == 0) {
      pln::utils::crash("Can't create a program.");
    }
  }


  GLuint
  compile_shader(std::string& text, unsigned int shader_type)
  {
    auto shader = glCreateShader(shader_type);
    if (shader == 0) {
      pln::utils::crash("Can't create a shader.");
    }

    auto shader_text = text.c_str();
    glShaderSource(shader, 1, &shader_text, nullptr);
    glCompileShader(shader);

    crash_on_shader_error(shader, GL_COMPILE_STATUS);

    glAttachShader(_program, shader);

    return shader;
  }


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
      pln::utils::crash(log_text);
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
      pln::utils::crash(log_text);
    }
  }
};

}
