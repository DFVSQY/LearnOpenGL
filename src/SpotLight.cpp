#include "SpotLight.h"
#include "BaseLight.h"
#include "glm/fwd.hpp"

SpotLight::SpotLight(const glm::vec3 &position, const glm::vec3 &direction, const GLfloat cutOff,
                     const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular)
    : BaseLight(ambient, diffuse, specular), m_position(position), m_direction(direction), m_cutOff(cutOff)
{
}

SpotLight::~SpotLight()
{
}