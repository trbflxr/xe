out vec4 color;

in vec2 v_texCoords;

uniform sampler2D u_tex2d0;
uniform sampler2D u_tex2d1;

//uniform float u_alpha;
uniform vec4 u_tint;

void main() {
  if (v_texCoords.x <= 0.5) {
    color = u_tint * texture(u_tex2d0, v_texCoords);
  } else {
    color = texture(u_tex2d1, v_texCoords);
  }
}
