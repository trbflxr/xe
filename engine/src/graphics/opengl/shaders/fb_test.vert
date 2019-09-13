in vec3 a_position;
in vec2 a_texCoords;

out vec2 v_texCoords;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
  gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
  v_texCoords = a_texCoords;
}
