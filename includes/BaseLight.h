#pragma once

#include "glm/glm.hpp"

class BaseLight
{
  protected:
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;

  public:
    BaseLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular);
    virtual ~BaseLight();

    // 禁止拷贝和赋值
    BaseLight(const BaseLight &) = delete;
    BaseLight &operator=(const BaseLight &) = delete;
};