#include <array>
#include <format>
#include <sstream>
#include <string>

#include "src/services/logger.h"

#include "src/utils/gl.h"

import pln.utils.crash;

int
buffer_size()
{
  GLint size = 0;
  glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
  return size;
}

/**
 * It should be used with OpenGL functions that returns text data.
 * For instance: glGetStringi
 */
std::string
glubyte_to_string(const GLubyte* value)
{
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  return { reinterpret_cast<const char*>(value) };
}

/**
 * Thanks to the documentation: https://www.khronos.org/opengl/wiki/OpenGL_Error
 */
void
print_opengl_errors(const std::string& place)
{
  GLenum error = 0;
  while ((error = glGetError()) != GL_NO_ERROR) {
    std::stringstream ss;
    ss << place << ": ";
    switch (error) {
      case GL_INVALID_ENUM: {
        ss << "An enumeration parameter is not legal (GL_INVALID_ENUM)";
        break;
      }
      case GL_INVALID_VALUE: {
        ss << "A value parameter is not a legal value (GL_INVALID_VALUE)";
        break;
      }
      case GL_INVALID_OPERATION: {
        ss << "The set of state for a command is not legal "
              "(GL_INVALID_OPERATION)";
        break;
      }
      case GL_STACK_OVERFLOW: {
        ss << "A stack pushing operation cannot be done because it would "
              "overflow the limit of that stack's size (GL_STACK_OVERFLOW)";
        break;
      }
      case GL_STACK_UNDERFLOW: {
        ss << "A stack popping operation cannot be done because the stack is "
              "already at its lowest point (GL_STACK_UNDERFLOW)";
        break;
      }
      case GL_OUT_OF_MEMORY: {
        ss << "The memory cannot be allocated (GL_OUT_OF_MEMORY)";
        break;
      }
      case GL_INVALID_FRAMEBUFFER_OPERATION: {
        ss << "Reading from or writeing/rendering to a framebuffer that is not "
              "complete (GL_INVALID_FRAMEBUFFER_OPERATION)";
        break;
      }
      case GL_CONTEXT_LOST: {
        ss << "The OpenGL context has been lost (GL_CONTEXT_LOST)";
        break;
      }
      default: {
        ss << "Unknown error: " << error;
        break;
      }
    }
    Services::logger().error(ss.str());
  }
}

void
print_opengl_info()
{
  Services::logger().info(std::format(
    "OpenGL version: {}", glubyte_to_string(glGetString(GL_VERSION))));
  Services::logger().info(
    std::format("Vendor: {}", glubyte_to_string(glGetString(GL_VENDOR))));
  Services::logger().info(
    std::format("Renderer: {}", glubyte_to_string(glGetString(GL_RENDERER))));
}

void
print_extension_support(std::string extension_name)
{
  int quantity = 0;
  glGetIntegerv(GL_NUM_EXTENSIONS, &quantity);
  for (int i = 0; i < quantity; i++) {
    std::string extension(glubyte_to_string(glGetStringi(GL_EXTENSIONS, i)));
    if (extension_name == extension) {
      Services::logger().info(std::format("{}: is supported", extension_name));
      return;
    }
  }
  Services::logger().info(std::format("{}: is not supported", extension_name));
  return;
}

GLuint
gen_color_attachment(GLint internal_color_format,
                     GLint color_attachment,
                     unsigned int width,
                     unsigned int height,
                     GLuint date_type)
{
  GLuint texture_handle = 0;
  glGenTextures(1, &texture_handle);
  glBindTexture(GL_TEXTURE_2D, texture_handle);
  glTexImage2D(GL_TEXTURE_2D,
               0,
               internal_color_format,
               static_cast<GLint>(width),
               static_cast<GLint>(height),
               0,
               GL_RGBA,
               date_type,
               nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glFramebufferTexture2D(
    GL_DRAW_FRAMEBUFFER, color_attachment, GL_TEXTURE_2D, texture_handle, 0);
  return texture_handle;
}

GLuint
gen_render_buffer(unsigned int width, unsigned int height)
{
  GLuint render_buffer = 0;
  glGenRenderbuffers(1, &render_buffer);
  glBindRenderbuffer(GL_RENDERBUFFER, render_buffer);
  glRenderbufferStorage(GL_RENDERBUFFER,
                        GL_DEPTH24_STENCIL8,
                        static_cast<GLsizei>(width),
                        static_cast<GLsizei>(height));
  glFramebufferRenderbuffer(
    GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, render_buffer);

  auto status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    std::array<GLint, 4> dimentions { 0, 0, 0, 0 };
    glGetIntegerv(GL_VIEWPORT, dimentions.data());
    Services::logger().error(std::format(
      "Viewport dimentions: {}x{}", dimentions.at(2), dimentions.at(3)));
    auto status_text = std::format(
      "Framebuffer status ({}x{}): {}", width, height, status);
    pln::utils::crash(status_text);
  }
  return render_buffer;
}
