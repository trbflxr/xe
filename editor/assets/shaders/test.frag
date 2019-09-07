#version 330 core

out vec4 color;

in vec4 v_color;
in vec2 v_texCoords;

uniform sampler2D u_tex2d0;

void main() {
  color = v_color * texture(u_tex2d0, v_texCoords);
  //color = texture(u_tex2d0, v_texCoords);
}
