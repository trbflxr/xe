//
// Created by FLXR on 9/8/2019.
//

#include "xepch.hpp"
#include "gl_shader_parser.hpp"
#include <xe/graphics/render_context.hpp>

namespace xe::gpu {

  static VertexFormat::Enum stringToShaderDataTypeGL(std::string_view type) {
    if (type == "float") return VertexFormat::Float;
    if (type == "vec2") return VertexFormat::Float2;
    if (type == "vec3") return VertexFormat::Float3;
    if (type == "vec4") return VertexFormat::Float4;
    if (type == "mat4") return VertexFormat::Mat4;
    if (type == "int") return VertexFormat::Int32;
    return VertexFormat::Undefined;
  }

  bool GLShaderParser::parse(std::string_view source, Backend::Uniform *uniforms, size_t &outUsedUniforms, size_t &outSize) {
    const char *token;
    const char *sourcePtr;

    //uniforms
    size_t index = outUsedUniforms;
    sourcePtr = source.data();
    while ((token = string::findToken(sourcePtr, "uniform"))) {
      if (index >= cMaxShaderUniforms) return false;
      parseUniform(string::getStatement(token, &sourcePtr), uniforms, index, outSize);
    }
    outUsedUniforms += index;
    return true;
  }

  void GLShaderParser::parseUniform(std::string_view statement, Backend::Uniform *uniforms, size_t &index, size_t &outSize) {
    auto tokens = string::tokenize(statement);
    size_t i = tokens.size() - 1;

    if (tokens[2][0] == '{') return; // uniform buffer

    std::string name = tokens[i--];
    const std::string typeStr = tokens[i];

    // strip ; from Name if present
    if (const char *s = strstr(name.c_str(), ";")) {
      name = std::string(name.c_str(), s - name.c_str());
    }

    const VertexFormat::Enum t = stringToShaderDataTypeGL(typeStr);
    if (t == VertexFormat::Undefined) return;

    uint32_t offset = 0;
    if (index > 0) {
      offset = uniforms[index - 1].offset + uniforms[index - 1].size;
    }
    const uint32_t size = RenderContext::computeSize(t);
    outSize += size;
    uniforms[index++] = {-1, name, t, size, offset};
  }

}
