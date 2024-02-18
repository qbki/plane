#version 300 es
precision highp float;

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coord;
layout (location = 3) in uint a_texture_id;
layout (location = 4) in mat4 a_model_transform;

uniform mat4 u_PV;
uniform mat3 u_normal_matrix;

out vec3 i_position;
out vec3 i_normal;
flat out uint i_texture_id;
out vec2 i_tex_coord;

void main() {
  vec3 position = vec3(a_model_transform * vec4(a_position, 1.0));
  mat3 normal_matrix = transpose(inverse(mat3(a_model_transform)));
  i_position = position;
  i_normal = normal_matrix * a_normal;
  i_texture_id = a_texture_id;
  i_tex_coord = a_tex_coord;
  gl_Position = u_PV * vec4(position, 1.0);
}
