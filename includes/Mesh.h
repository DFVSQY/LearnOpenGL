#pragma once

#include "Material.h"
#include "glad/glad.h"

class Mesh
{
  private:
    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    GLsizei index_num; // 索引缓冲区中索引的数量

    Material *material;

    void SetupVAO(const GLfloat *vertices, const int vertexNum);

    void SetupVAO_Elements(const GLfloat *vertices, const int vertexNum, const GLuint *indices, const int indexNum);

  public:
    Mesh();
    ~Mesh();
    bool Init(const GLfloat *vertices, const int vertexNum);

    bool Init_Elements(const GLfloat *vertices, const GLuint *indices, const int vertexNum, const int indexNum,
                       Material *material);

    void Draw() const;
};