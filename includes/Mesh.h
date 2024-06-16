#pragma once

#include "glad/glad.h"
#include "Shader.h"

class Mesh
{
  private:
    GLuint vao;
    GLuint vbo;

    GLuint shader_program;

    void SetupVAO(const float *vertices, const int vertexNum);

  public:
    Mesh();
    ~Mesh();

    bool Init(const float *vertices, const int vertexNum, const GLuint shader_program);

    void Draw() const;
};