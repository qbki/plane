#version 450 core
precision highp float;

layout (location = 0) out vec3 g_position;
layout (location = 1) out vec3 g_normal;
layout (location = 2) out vec3 g_base_color;

in InterfaceData {
  flat vec3 position;
  flat vec3 normal;
  vec2 tex_coord;
} interface_data;

uniform sampler2D u_base_color_texture;

void main() {
  vec4 color_sample = texture(u_base_color_texture, interface_data.tex_coord);

  if (color_sample.a < 0.1) {
    discard;
  }

  g_position = interface_data.position;
  g_normal = normalize(interface_data.normal);
  g_base_color = color_sample.rgb;
}
