#pragma once

#include "Mesh.h"

class Cube : public Mesh
{
  protected:
    /* 半径 */
    GLfloat radius;

  public:
    Cube(Shader &shader, GLfloat radius = 0.5f);
    ~Cube();
};