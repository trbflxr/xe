layout(location = 0) in vec2 a_position;
layout(location = 1) in vec2 a_texCoords;
layout(location = 2) in vec4 a_color;
layout(location = 3) in vec4 a_outlineColor;
layout(location = 4) in vec2 a_texHandle;
layout(location = 5) in vec2 a_fontData;

layout(std140, row_major) uniform Camera2DUniform {
  mat4 proj;
  mat4 view;
} cam;

out vec2 v_texCoords;
out vec4 v_color;
out vec4 v_outlineColor;
out vec2 v_texHandle;
out vec2 v_fontData;

void main() {
  gl_Position = cam.proj * cam.view * vec4(a_position, 0.0, 1.0);

  v_texCoords = a_texCoords;
  v_color = a_color;
  v_outlineColor = a_outlineColor;
  v_texHandle = a_texHandle;
  v_fontData = a_fontData;
}
