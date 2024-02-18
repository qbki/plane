#include <format>
#include <sstream>
#include <string>

#include "src/services.h"

#include "gl.h"

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
    logger().error(ss.str());
  }
}

void
print_opengl_info()
{
  logger().info(std::format("OpenGL version: {}",
                            glubyte_to_string(glGetString(GL_VERSION))));
  logger().info(
    std::format("Vendor: {}", glubyte_to_string(glGetString(GL_VENDOR))));
  logger().info(
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
      logger().info(std::format("{}: is supported", extension_name));
      return;
    }
  }
  logger().info(std::format("{}: is not supported", extension_name));
  return;
}
