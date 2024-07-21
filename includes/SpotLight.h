#pragma once

#include "BaseLight.h"

class SpotLight : public BaseLight
{
  protected:
    glm::vec3 m_position;

  public:
    SpotLight(const glm::vec3 &position, const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular);
    ~SpotLight();
};