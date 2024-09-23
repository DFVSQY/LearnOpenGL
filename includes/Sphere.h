#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include <vector>

class Sphere : public Mesh
{
  protected:
    /* 半径 */
    float radius;

    /* 经度 */
    unsigned int longitude_divs;

    /* 纬度 */
    unsigned int latitude_divs;

    /* 生成顶点数据 */
    void GenMeshData(std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);

  public:
    Sphere(Shader &shader, float radius = 1.0f, unsigned int longitudeDivs = 30, unsigned int latitudeDivs = 30);
    ~Sphere();
};