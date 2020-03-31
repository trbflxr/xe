layout(std140, row_major) uniform StateUniform {
  mat4 cubeModel;
  mat4 cubeView;
  mat4 cubeProj;
} state;
