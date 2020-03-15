layout(location = 0) in vec2 a_position;
layout(location = 1) in vec2 a_texCoords;
layout(location = 2) in vec4 a_color;

layout(location = 0) uniform mat4 u_proj;
layout(location = 1) uniform mat4 u_view;

out vec2 v_texCoords;
out vec4 v_color;

void main() {
  gl_Position = u_proj * u_view * vec4(a_position, 0.0, 1.0);

  v_texCoords = a_texCoords;
  v_color = a_color;
}
