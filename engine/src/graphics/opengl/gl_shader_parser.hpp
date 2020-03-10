//
// Created by FLXR on 9/8/2019.
//

#ifndef XE_GL_SHADER_PARSER_HPP
#define XE_GL_SHADER_PARSER_HPP

#include "gl_backend.hpp"

namespace xe::gpu {

  class GLShaderParser : NonCopyable {
  public:
    static bool parse(std::string_view source, Backend::Uniform *uniforms, uint32_t &outUsedUniforms, uint32_t &outSize);

  private:
    static void parseUniform(std::string_view statement, Backend::Uniform *uniforms, uint32_t &index, uint32_t &outSize);
  };

}

#endif //XE_GL_SHADER_PARSER_HPP
