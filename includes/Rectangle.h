#pragma once

#include "Mesh.h"
#include "Texture.h"

class Rectangle : public Mesh
{
  protected:
    GLfloat width, height;

  public:
    Rectangle(Shader &shader, GLfloat width = 1.0f, GLfloat height = 1.0f);
    ~Rectangle();
};