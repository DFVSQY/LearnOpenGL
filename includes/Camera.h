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

    double m_near;
    double m_far;
    double m_aspect;

  public:
    // 删除复制构造函数和赋值操作符
    Camera(const Camera &) = delete;
    Camera &operator=(const Camera &) = delete;

    Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), double fov = 45.0f, double aspect = 800.0 / 600.0,
           double near = 0.1f, double far = 100.0f);

    ~Camera();

    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();

    glm::vec3 GetPos() const;
    glm::vec3 GetFront() const;

    void MoveForwardOrBackward(float delta);
    void MoveLeftOrRight(float delta);
    void TuneYawAndPitch(float deltaYaw, float deltaPitch);
    void TuneZoom(float delta);
};