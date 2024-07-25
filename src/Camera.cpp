#include "Camera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

Camera::Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up, double fov, double aspect, double near, double far)
    : m_pos(pos), m_front(front), m_up(up), m_fov(fov), m_near(near), m_far(far), m_aspect(aspect)
{
    /*
     * 相机初始朝向设定为看向-z轴，所以初始yaw为-90度。
    */
    m_yaw = -90.0;
    m_pitch = 0.0;
}

Camera::~Camera()
{
}

glm::vec3 Camera::GetPos() const
{
    return m_pos;
}

glm::vec3 Camera::GetFront() const
{
    return m_front;
}

glm::mat4 Camera::GetViewMatrix()
{
    /*
     * 通过lookAt函数创建视图矩阵。

     * 函数原型：
     *  glm::mat4 glm::lookAt(glm::vec3 const& eye, glm::vec3 const& center, glm::vec3 const& up);
     * 函数参数：
     *  glm::vec3 const& eye: 相机的位置
     *  glm::vec3 const& center: 相机看向的目标点
     *  glm::vec3 const& up: 世界空间中的上方向向量

     * 基本实现原理：
     * glm::mat4 myLookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up)
     * {
     *    glm::vec3 F = glm::normalize(center - eye);
     *    glm::vec3 R = glm::normalize(glm::cross(F, up));
     *    glm::vec3 U = glm::cross(R, F);
     *    return glm::mat4(
     *        R.x, U.x, -F.x, 0,                                                    // 第一列
     *        R.y, U.y, -F.y, 0,                                                    // 第二列
     *        R.z, U.z, -F.z, 0,                                                    // 第三列
     *        -glm::dot(R, eye), -glm::dot(U, eye), glm::dot(F, eye), 1             // 第四列
     *    );
     * }
    */
    glm::mat4 view = glm::lookAt(m_pos, m_pos + m_front, m_up);
    return view;
}

glm::mat4 Camera::GetProjectionMatrix()
{
    /*
     * 设置投影矩阵为透视投影矩阵，参数分别为：
     * 视野角度45度，宽高比800/600，近裁剪面0.1，远裁剪面100。
     * 这定义了一个视锥体，只有在这个视锥体内的对象才会被渲染。
    */
    glm::mat4 projection = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
    return projection;
}

void Camera::MoveForwardOrBackward(float delta)
{
    m_pos += glm::normalize(m_front) * delta;
}

void Camera::MoveLeftOrRight(float delta)
{
    m_pos += glm::normalize(glm::cross(m_front, m_up)) * delta;
}

void Camera::TuneYawAndPitch(float deltaYaw, float deltaPitch)
{
    m_yaw += deltaYaw;
    m_pitch += deltaPitch;
    m_pitch = glm::clamp(m_pitch, -89.0, 89.0);

    m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front.y = sin(glm::radians(m_pitch));
    m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_front = glm::normalize(m_front);
}

void Camera::TuneZoom(float delta)
{
    m_fov -= delta;
    m_fov = glm::clamp(m_fov, 1.0, 60.0);
}