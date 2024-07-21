#pragma once

#include "BaseLight.h"

class DirectionLight : public BaseLight
{
  protected:
    glm::vec3 m_direction;

  public:
    DirectionLight(const glm::vec3 &direction, const glm::vec3 &ambient, const glm::vec3 &diffuse,
                   const glm::vec3 &specular);
    ~DirectionLight();
};