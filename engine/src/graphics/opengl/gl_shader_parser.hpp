//
// Created by FLXR on 9/8/2019.
//

#ifndef XE_GL_SHADER_PARSER_HPP
#define XE_GL_SHADER_PARSER_HPP

#include "gl_backend.hpp"

namespace xe::gpu {

  class GLShaderParser : NonCopyable {
  public:
    static bool parse(const string &source, Backend::Uniform *uniforms, size_t &outUsedUniforms, size_t &outSize);

  private:
    static void parseUniform(const string &statement, Backend::Uniform *uniforms, size_t &index, size_t &outSize);
  };

}

#endif //XE_GL_SHADER_PARSER_HPP
