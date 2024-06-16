#pragma once

#include "glad/glad.h"

class Shader
{
  private:
    GLuint shader_program;

    const char *vertex_file_path;

    const char *fragment_file_path;

    const char *ReadShaderContent(const char *filePath);

    GLuint Compile(GLenum shaderType);

    GLuint Link(GLuint vertexShader, GLuint fragmentShader);

  public:
    Shader();

    GLuint Init(const char *vertexFilePath, const char *fragmentFilePath);

    GLuint GetShaderProgram() const;

    ~Shader();
};