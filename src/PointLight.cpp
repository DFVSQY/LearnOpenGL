#include "PointLight.h"
#include "BaseLight.h"

PointLight::PointLight(const glm::vec3 &position, const glm::vec3 &ambient, const glm::vec3 &diffuse,
                       const glm::vec3 &specular)
    : BaseLight(ambient, diffuse, specular), m_position(position)
{
}

PointLight::~PointLight()
{
}