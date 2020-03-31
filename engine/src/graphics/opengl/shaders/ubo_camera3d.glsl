layout(std140, row_major) uniform CameraUniform {
  mat4 view;
  mat4 proj;
  vec4 resolution;
} camera;
