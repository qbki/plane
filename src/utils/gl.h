#pragma once
#include <GL/glew.h>
#include <string>

int
buffer_size();

/**
 * HACK: emscripten's libc++ doesn't support custom types for std::format
 * well. Therefore, you should use this function for the GLubyte* type.
 * Use this after emscripten updates libc++:
 *
 * template <>
 * struct std::formatter<const GLubyte*> : std::formatter<std::string> {
 *   auto format(const GLubyte* text, format_context& ctx) const {
 *     auto reinterpreted_text = reinterpret_cast<const char*>(text);
 *     return formatter<string>::format(
 *       std::format("{}", reinterpreted_text),
 *       ctx
 *     );
 *   }
 * };
 */
std::string
glubyte_to_string(const GLubyte* value);

void
print_opengl_errors(const std::string& place);

void
print_opengl_info();

void
print_extension_support(std::string extension_name);

GLuint
gen_color_attachment(GLint internal_color_format,
                     GLint color_attachment,
                     unsigned int width,
                     unsigned int height,
                     GLuint date_type);

GLuint
gen_render_buffer(unsigned int width, unsigned int height);
