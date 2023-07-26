#version 450 core
precision highp float;

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;

uniform mat4 u_PV;
uniform mat4 u_M;
uniform mat3 u_normal_matrix;

out InterfaceData {
  flat vec3 position;
  flat vec3 normal;
} interface_data;

void main() {
  vec3 position = vec3(u_M * vec4(a_position, 1.0));
  interface_data.position = position;
  interface_data.normal = u_normal_matrix * a_normal;
  gl_Position = u_PV * vec4(position, 1.0);
}
