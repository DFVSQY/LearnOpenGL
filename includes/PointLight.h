#pragma once

#include "BaseLight.h"

class PointLight : public BaseLight
{
  protected:
    glm::vec3 m_position;

  public:
    PointLight(const glm::vec3 &position, const glm::vec3 &ambient, const glm::vec3 &diffuse,
               const glm::vec3 &specular);
    ~PointLight();
};