#pragma once

#include "BaseLight.h"
#include "glad/glad.h"

class PointLight : public BaseLight
{
  protected:
    glm::vec3 m_position;

    GLfloat m_constant;  // 常量系数
    GLfloat m_linear;    // 一次递减系数
    GLfloat m_quadratic; // 二次递减系数

  public:
    PointLight(const glm::vec3 &position, const GLfloat linear, const GLfloat quadratic, const glm::vec3 &ambient,
               const glm::vec3 &diffuse, const glm::vec3 &specular, const GLfloat constant);
    ~PointLight();
};