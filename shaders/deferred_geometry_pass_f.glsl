#version 300 es
precision highp float;

layout (location = 0) out vec3 g_position;
layout (location = 1) out vec3 g_normal;
layout (location = 2) out vec3 g_base_color;

in vec3 i_position;
in vec3 i_normal;
in vec2 i_tex_coord;

uniform sampler2D u_texture;

void main() {
  vec4 color_sample = texture(u_texture, i_tex_coord);
  g_position = i_position;
  g_normal = normalize(i_normal);
  g_base_color = color_sample.rgb;
}
