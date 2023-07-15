#version 450 core
precision highp float;

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;

uniform mat4 u_PVM;
uniform mat3 u_normal_matrix;

out InterfaceData {
  vec3 position;
  vec3 normal;
} interface_data;

void main() {
  interface_data.position = a_position;
  interface_data.normal = u_normal_matrix * a_normal;
  gl_Position = u_PVM * vec4(a_position, 1.0);
}
