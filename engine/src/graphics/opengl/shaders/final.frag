layout(location = 0) out vec4 color;

in vec2 v_texCoords;

uniform sampler2D u_tex2d0;

vec3 simpleHDR(vec3 color) {
  float exposure = 2.2;
  float gamma = 1.0;
  vec3 mapped = vec3(1.0) - exp(-color * exposure);
  mapped = pow(mapped, vec3(1.0 / gamma));
  return mapped;
}

void main() {
  vec3 scene = texture(u_tex2d0, v_texCoords).xyz;

//  color = vec4(simpleHDR(scene), 1.0);
  color = vec4(scene, 1.0);
}
