#extension GL_ARB_bindless_texture : require

layout(std140, row_major) uniform PBRMaterial {
  vec4 mask;
  uvec2 diffuse;
  uvec2 normals;
} pbr;
