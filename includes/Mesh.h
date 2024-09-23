#pragma once

#include "Shader.h"
#include "glad/glad.h"
#include "VertexAttribute.h"

class Mesh
{
  protected:
    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    GLsizei index_num; // 索引缓冲区中索引的数量

    Shader *shader;

    void SetupMesh(const std::vector<GLfloat> &vertices, const std::vector<GLuint> &indices,
                   const std::vector<VertexAttribute> &attributes);

    /* 只希望在子类中调用 */
    Mesh(Shader *shader);

  public:
    ~Mesh();

    // 禁止复制构造函数和赋值
    Mesh(const Mesh &) = delete;
    Mesh &operator=(const Mesh &) = delete;

    Mesh(const std::vector<GLfloat> &vertices, const std::vector<GLuint> &indices,
         const std::vector<VertexAttribute> &attributes, Shader *shader);

    void Draw() const;

    Shader &GetShader() const;

    void ChangeShader(Shader *shader);
};