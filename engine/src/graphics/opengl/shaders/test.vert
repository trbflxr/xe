in vec3 a_position;
in vec4 a_color;
in vec2 a_texCoords;
in vec4 a_instancePosition;

out vec4 v_color;
out vec2 v_texCoords;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
  gl_Position = u_projection * u_view * u_model * vec4(a_position + a_instancePosition.xyz, 1.0);
  v_color = a_color;
  v_texCoords = a_texCoords;
}
