layout(location = 0) out vec4 color;

in vec2 v_texCoords;

void main() {
  vec4 final = pbr.mask;
  if (pbr.diffuse.x > 0 || pbr.diffuse.y > 0) {
    final *= texture(sampler2D(pbr.diffuse), v_texCoords);
    final *= texture(sampler2D(pbr.normals), v_texCoords);
  }
  color = final;
}
