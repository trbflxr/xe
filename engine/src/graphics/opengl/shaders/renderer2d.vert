layout(location = 0) in vec2 a_position;
layout(location = 1) in vec2 a_texCoords;
layout(location = 2) in vec4 a_color;
layout(location = 3) in vec2 a_texHandle;
layout(location = 4) in int a_useTexture;

layout(std140, row_major) uniform Camera2DUniform {
  mat4 proj;
  mat4 view;
} cam;

out vec2 v_texCoords;
out vec4 v_color;
out vec2 v_texHandle;
flat out int v_useTexture;

void main() {
  gl_Position = cam.proj * cam.view * vec4(a_position, 0.0, 1.0);

  v_texCoords = a_texCoords;
  v_color = a_color;
  v_texHandle = a_texHandle;
  v_useTexture = a_useTexture;
}
