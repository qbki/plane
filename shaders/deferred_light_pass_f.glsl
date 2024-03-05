#version 300 es
#define MAX_POINT_LIGHTS_QUANTITY 64
precision highp float;

struct Material {
  vec3 ambient;
  vec3 specular;
  float shininess;
};

struct DirectionalLight {
  vec3 color;
  vec3 direction;
};

struct PointLight {
  vec3 color;
  vec3 position;
  float constant;
  float linear;
  float quadratic;
};

in vec2 i_tex_coord;

out vec4 frag_color;

uniform sampler2D u_position_texture;
uniform sampler2D u_normal_texture;
uniform sampler2D u_base_color_texture;

uniform DirectionalLight u_light;
uniform PointLight u_point_lights[MAX_POINT_LIGHTS_QUANTITY];
uniform int u_point_lights_quantity;

uniform vec3 u_camera_pos;
uniform Material u_material;
uniform float u_gamma;

vec3 encode_gamma(vec3 color) {
  return pow(color, vec3(1.0 / u_gamma));
}

vec3 calc_diffuse(vec3 light_color, vec3 surface_color, vec3 surface_normal, vec3 light_direction) {
  float light_angle = max(dot(surface_normal, -light_direction), 0.0);
  return light_color * surface_color * light_angle;
}

vec3 calc_specular(
  vec3 light_color,
  vec3 surface_normal,
  vec3 light_direction,
  vec3 view_direction,
  float shininess
) {
  vec3 reflected_light_direction = reflect(light_direction, surface_normal);
  float power = pow(
      max(dot(view_direction, reflected_light_direction),
          0.0),
      shininess);
  return light_color * power;
}

float calc_attenuation(PointLight light, float distance) {
  return (1.0 / (
      light.constant
      + light.linear * distance
      + light.quadratic * distance * distance));
}

void main() {
  vec3 base_color = texture(u_base_color_texture, i_tex_coord).rgb;
  vec3 pixel_position = texture(u_position_texture, i_tex_coord).xyz;
  vec3 normal = normalize(texture(u_normal_texture, i_tex_coord).xyz);
  vec3 view_direction = normalize(u_camera_pos - pixel_position);
  vec3 light_direction = normalize(u_light.direction);

  vec3 ambient = vec3(0.0);
  vec3 diffuse = vec3(0.0);
  vec3 specular = vec3(0.0);

  ambient += base_color.rgb * u_material.ambient;

  diffuse += calc_diffuse(
    u_light.color,
    base_color,
    normal,
    light_direction);

  specular += calc_specular(
    u_light.color,
    normal,
    light_direction,
    view_direction,
    u_material.shininess
  ) * u_material.specular;

  int lights_quantity = min(MAX_POINT_LIGHTS_QUANTITY, u_point_lights_quantity);
  for (int i = 0; i < lights_quantity; i++) {
    vec3 light_pos = u_point_lights[i].position;
    vec3 light_color = u_point_lights[i].color;
    vec3 point_light_direction = normalize(pixel_position - light_pos);
    float ray_distance = length(light_pos - pixel_position);
    float attenuation = calc_attenuation(u_point_lights[i], ray_distance);

    diffuse += calc_diffuse(
      light_color,
      base_color,
      normal,
      point_light_direction) * attenuation;

    specular += calc_specular(
      light_color,
      normal,
      point_light_direction,
      view_direction,
      u_material.shininess
    ) * u_material.specular * attenuation;
  }

  vec3 result = encode_gamma(ambient + diffuse + specular);
  frag_color = vec4(result, 1.0);
}
