#pragma once

#include "BaseLight.h"
#include "glad/glad.h"

class SpotLight : public BaseLight
{
  protected:
    glm::vec3 m_position;  // 灯光位置
    glm::vec3 m_direction; // 灯光方向
    GLfloat m_cutOff;      // 切光角

  public:
    SpotLight(const glm::vec3 &position, const glm::vec3 &direction, const GLfloat cutOff, const glm::vec3 &ambient,
              const glm::vec3 &diffuse, const glm::vec3 &specular);
    ~SpotLight();
};