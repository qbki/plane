#version 300 es
precision highp float;

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coord;

uniform mat4 u_PV;
uniform mat4 u_M;
uniform mat3 u_normal_matrix;

out vec3 i_position;
out vec3 i_normal;
out vec2 i_tex_coord;

void main() {
  vec3 position = vec3(u_M * vec4(a_position, 1.0));
  i_position = position;
  i_normal = u_normal_matrix * a_normal;
  i_tex_coord = a_tex_coord;
  gl_Position = u_PV * vec4(position, 1.0);
}
