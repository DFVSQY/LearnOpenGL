#pragma once

#include "glad/glad.h"
#include <string>

class ShaderUnit
{
  private:
    GLuint shader_id;

    const std::string ReadShaderFile(const std::string &path) const;

    GLuint Compile(GLenum shaderType, const std::string &shaderCode);

  public:
    // 删除复制构造函数和赋值操作符
    ShaderUnit(const ShaderUnit &) = delete;
    ShaderUnit &operator=(const ShaderUnit &) = delete;

    ShaderUnit(const std::string &path, const GLenum shaderType);
    ~ShaderUnit();

    GLuint GetShaderID() const;
};