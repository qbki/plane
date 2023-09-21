#version 450 core
precision highp float;

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coord;

uniform mat4 u_PV;
uniform mat4 u_M;
uniform mat3 u_normal_matrix;

out InterfaceData {
  flat vec3 position;
  flat vec3 normal;
  vec2 tex_coord;
} interface_data;

void main() {
  vec3 position = vec3(u_M * vec4(a_position, 1.0));
  interface_data.position = position;
  interface_data.normal = u_normal_matrix * a_normal;
  interface_data.tex_coord = a_tex_coord;
  gl_Position = u_PV * vec4(position, 1.0);
}
