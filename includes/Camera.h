#pragma once

#include "glm/glm.hpp"

class Camera
{
  private:
    glm::vec3 m_pos;
    glm::vec3 m_front;
    glm::vec3 m_up;

    double m_yaw;
    double m_pitch;
    double m_fov;

  public:
    // 删除复制构造函数和赋值操作符
    Camera(const Camera &) = delete;
    Camera &operator=(const Camera &) = delete;

    Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), double fov = 45.0f);

    ~Camera();

    glm::mat4 GetViewMatrix();
};