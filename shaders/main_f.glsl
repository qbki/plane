#version 450 core
precision highp float;

in InterfaceData  {
  vec3 position;
  vec3 normal;
} interface_data;

out vec4 frag_color;

uniform vec3 camera_pos;

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

void main() {
  Material material = Material(
    vec3(0.1, 0.1, 0.1),
    vec3(1.0, 0.5, 0.2),
    vec3(1.0, 1.0, 1.0),
    0.6
  );

  SunLight light = SunLight(
    vec3(1.0, 1.0, 1.0),
    normalize(vec3(0.0, 0.0, 1.0))
  );

  vec3 view_pos = vec3(0.0, 0.0, -1.0);
  vec3 view_dir = normalize(view_pos - interface_data.position);

  vec3 ambient = light.color * material.ambient;

  float angle = max(dot(interface_data.normal, light.direction), 0.0);
  vec3 diffuse = light.color * (angle * material.diffuse);

  vec3 reflect_dir = reflect(-light.direction, interface_data.normal);
  float power = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
  vec3 specular = light.color * (power * material.specular);

  vec3 result = ambient + diffuse + specular;

  frag_color = vec4(result, 1.0f);
}
