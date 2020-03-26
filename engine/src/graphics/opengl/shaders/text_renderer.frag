#extension GL_ARB_bindless_texture : require

layout(location = 0) out vec4 color;

in vec2 v_texCoords;
in vec4 v_color;
in vec4 v_outlineColor;
in vec2 v_texHandle;

float median(float r, float g, float b) {
  return max(min(r, g), min(max(r, g), b));
}

void main() {
  vec3 fontSample = texture(sampler2D(uvec2(v_texHandle)), v_texCoords).rgb;

  float sigDist = median(fontSample.r, fontSample.g, fontSample.b);
  float w = fwidth(sigDist);

  float alpha = smoothstep(0.5 - w, 0.5 + w, sigDist);

  color = vec4(v_color.rgb, alpha);
}
