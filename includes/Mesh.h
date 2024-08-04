#pragma once

#include "Shader.h"
#include "glad/glad.h"
#include "VertexAttribute.h"

class Mesh
{
  private:
    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    GLsizei index_num; // 索引缓冲区中索引的数量

    Shader *shader;

    void SetupMesh(const std::vector<GLfloat> &vertices, const std::vector<GLuint> &indices,
                   const std::vector<VertexAttribute> &attributes);

  public:
    ~Mesh();

    Mesh(const std::vector<GLfloat> &vertices, const std::vector<GLuint> &indices,
         const std::vector<VertexAttribute> &attributes, Shader *shader);

    void Draw() const;

    Shader *GetShader() const;
};