layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texCoords;
layout(location = 2) in vec4 a_color;
layout(location = 3) in vec4 a_instancePosition;

out vec4 v_color;
out vec2 v_texCoords;

void main() {
  gl_Position = state.cubeProj * state.cubeView * state.cubeModel * vec4(a_position + a_instancePosition.xyz, 1.0);
  v_color = a_color;
  v_texCoords = a_texCoords;
}
