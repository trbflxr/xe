#extension GL_ARB_bindless_texture : require

layout(location = 0) out vec4 color;

in vec2 v_texCoords;
in vec4 v_color;
in vec2 v_texHandle;

void main() {
  color = texture(sampler2D(uvec2(v_texHandle)), v_texCoords) * v_color;
}
