#pragma once

#include <string>
#include "glad/glad.h"
#include "glm/glm.hpp"

class Shader
{
  private:
    GLuint shader_program;

    std::string ReadShaderFile(const char *filePath);

    GLuint Compile(GLenum shaderType, const char *fileContent);

    GLuint Link(GLuint vertexShader, GLuint fragmentShader);

    GLint GetUniformLocation(const std::string &name) const;

  public:
    // 删除复制构造函数和赋值操作符
    Shader(const Shader &) = delete;
    Shader &operator=(const Shader &) = delete;

    Shader();
    ~Shader();

    bool Init(const char *vertexFilePath, const char *fragmentFilePath);

    void SetBool(const std::string &name, const GLboolean value) const;
    void SetInt(const std::string &name, const GLint value) const;
    void SetFloat(const std::string &name, const GLfloat value) const;
    void SetFloat4(const std::string &name, const GLfloat v0, const GLfloat v1, const GLfloat v2,
                   const GLfloat v3) const;
    void SetMat4f(const std::string &name, const glm::mat4 &matrix) const;
    void SetMat3f(const std::string &name, const glm::mat3 &matrix) const;
    void SetVec3f(const std::string &name, const glm::vec3 &vector) const;
    void Use();
};