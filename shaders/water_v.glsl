#version 300 es
precision highp float;

layout (location = 0) in vec3 a_position;

uniform mat4 u_PV;
uniform mat4 u_M;
uniform mat3 u_normal_matrix;
uniform float u_elapsed_seconds;

out vec3 i_position;
out vec3 i_normal;
out vec2 i_tex_coord;

float OFFSET = 0.001;

vec3 sin_fn(vec3 value) {
  float x = value.x;
  float y = value.y;
  float speed = u_elapsed_seconds * 3.0;
  float height = 0.2 * (sin(x * 0.3 + speed)
                      + sin(x * 3.0 + speed) / (3.0 + y + speed)
                      + 0.2 * cos(y * 1.5 + x));
  return vec3(value.x, value.y, height);
}

void main() {
  vec3 position = vec3(u_M * vec4(a_position, 1.0));
  position = sin_fn(position);
  vec3 offset_a = sin_fn(vec3(position.x + OFFSET, position.y, position.z));
  vec3 offset_b = sin_fn(vec3(position.x, position.y + OFFSET, position.z));
  vec3 normal = normalize(cross(offset_a - position, offset_b - position));

  i_position = position;
  i_normal = u_normal_matrix * normal;

  gl_Position = u_PV * vec4(position, 1.0);
}
