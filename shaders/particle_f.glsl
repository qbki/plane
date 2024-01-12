#version 300 es
precision highp float;

in vec3 i_position;
in vec3 i_normal;
in vec2 i_tex_coord;

out vec4 frag_color;

uniform float u_gamma_in;
uniform float u_gamma_out;

uniform sampler2D u_main_texture;

vec3 encode_gamma(vec3 color) {
  return pow(color, vec3(1.0 / u_gamma_out));
}

vec3 decode_gamma(vec3 color) {
  return pow(color, vec3(u_gamma_in));
}

void main() {
  vec4 base_color_sample = texture(u_main_texture, i_tex_coord);
  float base_alpha = base_color_sample.a;
  vec3 base_color = decode_gamma(base_color_sample.rgb);
  frag_color = vec4(base_color, base_alpha);
}
