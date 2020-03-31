layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec3 a_tangent;
layout(location = 3) in vec3 a_biTangent;
layout(location = 4) in vec2 a_texCoords;

out vec2 v_texCoords;

void main() {
  gl_Position = camera.proj * camera.view * vec4(a_position, 1.0);
  v_texCoords = a_texCoords;
}
