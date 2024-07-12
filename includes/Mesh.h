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
    // 删除复制构造函数和赋值操作符
    Mesh(const Mesh &) = delete;
    Mesh &operator=(const Mesh &) = delete;

    ~Mesh();

    Mesh(const std::vector<GLfloat> &vertices, const std::vector<GLuint> &indices,
         const std::vector<VertexAttribute> &attributes, Material *material);

    void Draw() const;

    Material *GetMaterial() const;
};