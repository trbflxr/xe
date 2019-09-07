#version 330 core

in vec3 a_position;
in vec2 a_texCoords;

out vec2 v_texCoords;

layout(std140, row_major) uniform UniformState0 {
  mat4 model;
  mat4 view;
  mat4 proj;
};

void main() {
  gl_Position = proj * view * model * vec4(a_position, 1.0);
  v_texCoords = a_texCoords;
}
