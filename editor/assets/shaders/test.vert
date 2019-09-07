#version 330 core

in vec3 a_position;
in vec4 a_color;
in vec2 a_texCoords;
in vec4 a_instancePosition;

out vec4 v_color;
out vec2 v_texCoords;

layout(std140, row_major) uniform UniformState{
  mat4 model;
  mat4 view;
  mat4 proj;
};

void main() {
  gl_Position = proj * view * model * vec4(a_position + a_instancePosition.xyz, 1.0);
  v_color = a_color;
  v_texCoords = a_texCoords;
}
