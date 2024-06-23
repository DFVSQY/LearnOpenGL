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
    Shader();
    ~Shader();

    bool Init(const char *vertexFilePath, const char *fragmentFilePath);

    void SetBool(const std::string &name, bool value) const;
    void SetInt(const std::string &name, GLint value) const;
    void SetFloat(const std::string &name, GLfloat value) const;
    void SetFloat4(const std::string &name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) const;
    void SetMat4f(const std::string &name, const glm::mat4 &matrix) const;
    void Use();
};