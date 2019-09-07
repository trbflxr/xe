#version 330 core

out vec4 color;

in vec2 v_texCoords;

uniform sampler2D u_tex2d0;

void main() {
  color = texture(u_tex2d0, v_texCoords);
}
