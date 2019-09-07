#version 330 core

out vec4 color;

in vec2 v_texCoords;

uniform sampler2D u_tex2d0;
uniform sampler2D u_tex2d1;

void main() {
  if (v_texCoords.x <= 0.5){
    color = texture(u_tex2d0, v_texCoords);
  } else {
    color = texture(u_tex2d1, v_texCoords);
  }
}
