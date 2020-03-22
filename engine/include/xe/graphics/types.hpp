//
// Created by trbflxr on 3/11/2020.
//

#ifndef XE_TYPES_HPP
#define XE_TYPES_HPP

#include <xe/common.hpp>

namespace xe {

  enum class Usage {
    Static,
    Dynamic,
    Stream
  };

  enum class BufferType {
    Invalid,
    Vertex,
    Index,
    Uniform
  };

  enum class CubemapTarget {
    PositiveX,
    NegativeX,
    PositiveY,
    NegativeY,
    PositiveZ,
    NegativeZ,
    Invalid
  };

  enum class TexelsFormat {
    None,
    R8,
    Rg8,
    Rgb8,
    Rgba8,
    R16f,
    Rg16f,
    Rgb16f,
    Rgba16f,
    Depth16,
    DepthStencil16,
    Depth24,
    DepthStencil24
  };

  enum class TextureWrap {
    Repeat,
    MirroredRepeat,
    Clamp
  };

  enum class TextureMinFilter {
    Nearest,
    Linear,
    NearestMipmapNearest,
    NearestMipmapLinear,
    LinearMipmapNearest,
    LinearMipmapLinear
  };

  enum class TextureMagFilter {
    Nearest,
    Linear
  };

  enum class Primitive {
    Lines,
    Triangles,
    Points
  };

  enum class Cull {
    Disabled,
    Front,
    Back
  };

  enum class RenderMode {
    Disabled,
    Solid,
    Wireframe
  };

  enum class TextureType {
    Invalid = 0,
    T1D,
    T2D,
    T3D,
    CubeMap
  };

  enum class BlendFactor {
    Zero,
    One,
    SrcColor,
    OneMinusSrcColor,
    SrcAlpha,
    OneMinusSrcAlpha,
    DstColor,
    OneMinusDstColor,
    DstAlpha,
    OneMinusDstAlpha,
    SrcAlphaSaturated,
    BlendColor,
    OneMinusBlendColor,
    BlendAlpha,
    OneMinusBlendAlpha
  };

  enum class BlendOp {
    Add,
    Substract,
    ReverseSubstract,
    Min,
    Max
  };

  enum class CompareFunc {
    Disabled,
    Never,
    Less,
    LessEqual,
    Equal,
    NotEqual,
    GreaterEqual,
    Greater,
    Always
  };

  struct VertexFormat {
    enum Enum {
      Undefined = 0,
      Float = 0x1,
      Int8 = 0x2,
      Uint8 = 0x3,
      Int16 = 0x4,
      Uint16 = 0x5,
      Int32 = 0x6,
      Uint32 = 0x7,
      Mat4 = 0x8,

      NumComponents1 = 0x10,
      NumComponents2 = 0x20,
      NumComponents3 = 0x30,
      NumComponents4 = 0x40,

      Normalized = 0x100,

      Int32_1 = Int32 | NumComponents1,
      Uint32_2 = Uint32 | NumComponents2,
      Float1 = Float | NumComponents1,
      Float2 = Float | NumComponents2,
      Float3 = Float | NumComponents3,
      Float4 = Float | NumComponents4,

      TypeMask = 0xF,
      TypeShift = 0,
      NumComponentsMask = 0xF0,
      NumComponentsShift = 4,
      FlagsMask = 0xF00,
      FlagsShift = 8
    };
  };

  enum class VertexStep {
    PerVertex,
    PerInstance
  };

  struct VertexDeclaration {
    const char *name;
    VertexFormat::Enum format;
    uint32_t bufferIndex;
    VertexStep vertexStep;

    uint32_t offset;
    uint32_t stride;
  };

  enum class IndexFormat {
    Uint8,
    Uint16,
    Uint32
  };

}

#endif //XE_TYPES_HPP
