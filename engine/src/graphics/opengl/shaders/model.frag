layout(location = 0) out vec4 color;

in vec2 v_texCoords;

void main() {
  vec4 final = u_mask;

  if (u_diffuse.x > 0 || u_diffuse.y > 0) {
    final *= texture(sampler2D(u_diffuse), v_texCoords);
  }
  if (u_normals.x > 0 || u_normals.y > 0) {
    final *= texture(sampler2D(u_normals), v_texCoords);
  }
  color = final;
}
