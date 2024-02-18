#version 300 es
precision highp float;

layout (location = 0) out vec3 g_position;
layout (location = 1) out vec3 g_normal;
layout (location = 2) out vec3 g_base_color;

in vec3 i_position;
in vec3 i_normal;
flat in uint i_texture_id;
in vec2 i_tex_coord;

const uint TEXTURE_TYPE_PRIMARY = 0u;
const uint TEXTURE_TYPE_SECONDARY = 1u;
uniform sampler2D u_primary_texture;
uniform sampler2D u_secondary_texture;

void main() {
  vec4 color_sample;

  switch (i_texture_id) {
    case TEXTURE_TYPE_PRIMARY:
      color_sample = texture(u_primary_texture, i_tex_coord);
      break;
    case TEXTURE_TYPE_SECONDARY:
      color_sample = texture(u_secondary_texture, i_tex_coord);
      break;
  }

  if (color_sample.a < 0.1) {
    discard;
  }

  g_position = i_position;
  g_normal = normalize(i_normal);
  g_base_color = color_sample.rgb;
}
