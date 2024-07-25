#include "PointLight.h"
#include "BaseLight.h"

PointLight::PointLight(const glm::vec3 &position, const GLfloat linear, const GLfloat quadratic,
                       const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
                       const GLfloat constant = 1.0f)
    : BaseLight(ambient, diffuse, specular), m_position(position), m_constant(constant), m_linear(linear),
      m_quadratic(quadratic)
{
}

PointLight::~PointLight()
{
}