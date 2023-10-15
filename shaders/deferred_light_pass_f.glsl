#version 450 core
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

in vec2 i_tex_coord;

out vec4 frag_color;

uniform sampler2D u_position_texture;
uniform sampler2D u_normal_texture;
uniform sampler2D u_base_color_texture;

uniform vec3 u_camera_pos;
uniform SunLight u_light;
uniform Material u_material;
uniform float gamma_in;
uniform float gamma_out;

vec3 encode_gamma(vec3 color) {
  return pow(color, vec3(1.0 / gamma_out));
}

vec3 decode_gamma(vec3 color) {
  return pow(color, vec3(gamma_in));
}

void main() {
  vec3 base_color = decode_gamma(texture(u_base_color_texture, i_tex_coord).rgb);
  vec3 position = texture(u_position_texture, i_tex_coord).xyz;
  vec3 normal = normalize(texture(u_normal_texture, i_tex_coord).xyz);
  vec3 view_dir = normalize(u_camera_pos - position);
  vec3 light_dir = normalize(-u_light.direction);

  vec3 ambient = u_light.color * u_material.ambient * base_color;

  float light_angle = max(dot(normal, light_dir), 0.0);
  vec3 diffuse = base_color * light_angle;

  vec3 reflect_dir = reflect(-light_dir, normal);
  float power = pow(
      max(dot(view_dir, reflect_dir),
          0.0),
      u_material.shininess);
  vec3 specular = u_light.color * power * u_material.specular;

  vec3 result = ambient + diffuse + specular;

  frag_color = vec4(encode_gamma(result), 1.0);
}
