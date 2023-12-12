#version 300 es
precision highp float;

struct Material {
  vec3 ambient;
  vec3 specular;
  float shininess;
};

struct SunLight {
  vec3 color;
  vec3 direction;
};

in vec3 position;
in vec3 normal;
in vec2 tex_coord;

out vec4 frag_color;

uniform vec3 u_camera_pos;
uniform SunLight u_light;
uniform Material u_material;
uniform float gamma_in;
uniform float gamma_out;

uniform sampler2D main_texture;

vec3 encode_gamma(vec3 color) {
  return pow(color, vec3(1.0 / gamma_out));
}

vec3 decode_gamma(vec3 color) {
  return pow(color, vec3(gamma_in));
}

void main() {
  vec4 base_color_sample = texture(main_texture, i_tex_coord);
  float base_alpha = base_color_sample.a;
  vec3 base_color = decode_gamma(base_color_sample.rgb);

  if (base_alpha < 0.1) {
    discard;
  }

  vec3 view_dir = normalize(u_camera_pos - i_position);
  vec3 normal = normalize(i_normal);

  vec3 ambient = u_light.color * u_material.ambient;

  float angle = max(dot(normal, u_light.direction), 0.0);
  vec3 diffuse = base_color * angle;

  vec3 reflect_dir = reflect(-u_light.direction, normal);
  float power = pow(max(dot(view_dir, reflect_dir), 0.0), u_material.shininess);
  vec3 specular = u_light.color * (power * u_material.specular);

  vec3 result = ambient + diffuse + specular;

  frag_color = vec4(encode_gamma(result), 1.0);
}
