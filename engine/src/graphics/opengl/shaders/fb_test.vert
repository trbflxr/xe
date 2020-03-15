layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texCoords;

out vec2 v_texCoords;

layout(location = 0) uniform mat4 u_model;

void main() {
  gl_Position = camera.proj * camera.view * u_model * vec4(a_position, 1.0);
  v_texCoords = a_texCoords;
}
