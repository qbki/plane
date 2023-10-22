#version 450 core
precision highp float;

in InterfaceData {
  flat vec3 position;
  flat vec3 normal;
  vec2 tex_coord;
} interface_data;

out vec4 frag_color;

uniform float u_gamma_in;
uniform float u_gamma_out;

uniform sampler2D main_texture;

vec3 encode_gamma(vec3 color) {
  return pow(color, vec3(1.0 / u_gamma_out));
}

vec3 decode_gamma(vec3 color) {
  return pow(color, vec3(u_gamma_in));
}

void main() {
  vec4 base_color_sample = texture(main_texture, interface_data.tex_coord);
  float base_alpha = base_color_sample.a;
  vec3 base_color = decode_gamma(base_color_sample.rgb);
  frag_color = vec4(encode_gamma(base_color), base_alpha);
}
