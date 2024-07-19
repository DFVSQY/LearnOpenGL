#include "BaseLight.h"
#include "glm/fwd.hpp"

BaseLight::BaseLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular)
    : m_ambient(ambient), m_diffuse(diffuse), m_specular(specular)
{
}

BaseLight::~BaseLight()
{
}