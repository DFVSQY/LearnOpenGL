#include "Camera.h"
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"

Camera::Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up, double fov)
    : m_pos(pos), m_front(front), m_up(up), m_fov(fov)
{
    m_yaw = -90.0;
    m_pitch = 0.0;
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewMatrix()
{
    glm::mat4 view = glm::lookAt(m_pos, m_pos + m_front, m_up);
    return view;
}