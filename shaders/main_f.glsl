#version 450 core
precision highp float;

in InterfaceData  {
  vec3 position;
  vec3 normal;
} interface_data;

out vec4 frag_color;

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

struct SunLight {
  vec3 color;
  vec3 direction;
};

uniform vec3 u_camera_pos;
uniform SunLight u_light;

void main() {
  Material material = Material(
    vec3(0.1, 0.1, 0.1),
    vec3(1.0, 0.5, 0.2),
    vec3(1.0, 1.0, 1.0),
    30.0
  );

  vec3 view_dir = normalize(u_camera_pos - interface_data.position);

  vec3 ambient = u_light.color * material.ambient;

  float angle = max(dot(interface_data.normal, u_light.direction), 0.0);
  vec3 diffuse = u_light.color * (angle * material.diffuse);

  vec3 reflect_dir = reflect(-u_light.direction, interface_data.normal);
  float power = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
  vec3 specular = u_light.color * (power * material.specular);

  vec3 result = ambient + diffuse + specular;

  frag_color = vec4(result, 1.0f);
}
