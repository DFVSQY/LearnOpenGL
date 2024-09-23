#pragma once

#include "Mesh.h"

class Cube : public Mesh
{
  protected:
    /* 半径 */
    float radius;

  public:
    Cube(Shader &shader, float radius = 0.5f);
    ~Cube();
};