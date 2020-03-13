layout(std140, row_major, binding = 0) uniform StateUniform {
  mat4 cubeModel;
  mat4 cubeView;
  mat4 cubeProj;

  mat4 quadModel;
  mat4 quadView;
  mat4 quadProj;
};