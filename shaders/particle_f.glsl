#version 300 es
precision highp float;

in vec3 i_position;
in vec3 i_normal;
in vec2 i_tex_coord;

out vec4 frag_color;

uniform float u_gamma;

uniform sampler2D u_texture;

void main() {
  vec4 base_color = texture(u_texture, i_tex_coord);
  frag_color = vec4(base_color.rgb, base_color.a);
}
