#pragma once

#include "Material.h"
#include "glad/glad.h"
#include "VertexAttribute.h"

class Mesh
{
  private:
    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    GLsizei index_num; // 索引缓冲区中索引的数量

    Material *material;

    void SetupMesh(const std::vector<GLfloat> &vertices, const std::vector<GLuint> &indices,
                   const std::vector<VertexAttribute> &attributes);

  public:
    ~Mesh();

    Mesh(const std::vector<GLfloat> &vertices, const std::vector<GLuint> &indices,
         const std::vector<VertexAttribute> &attributes, Material *material);

    void Draw() const;

    Material *GetMaterial() const;
};