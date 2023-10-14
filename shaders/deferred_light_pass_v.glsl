#version 450 core
precision highp float;

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coord;

out vec2 i_tex_coord;

void main() {
  i_tex_coord = a_tex_coord;
  gl_Position = vec4(a_position, 1.0);
}
