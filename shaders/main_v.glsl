#version 450 core
precision highp float;

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;

uniform mat4 PV;

out InterfaceData  {
  vec3 position;
  vec3 normal;
} interface_data;

void main() {
  interface_data.position = aPosition;
  interface_data.normal = aNormal;
  gl_Position = PV * vec4(aPosition, 1.0);
}
