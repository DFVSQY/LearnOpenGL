#pragma once

#include "glad/glad.h"
#include "Shader.h"

class Mesh
{
  private:
    GLuint vao;
    GLuint vbo;
    const Shader *shader;

    void SetupVAO(const float *vertices, const int vertexNum);

  public:
    Mesh();
    ~Mesh();

    bool Init(const float *vertices, const int vertexNum, const Shader *shader);

    void Draw() const;
};