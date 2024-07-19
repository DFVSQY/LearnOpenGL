#include "DirectionLight.h"

DirectionLight::DirectionLight(const glm::vec3 &direction, const glm::vec3 &ambient, const glm::vec3 &diffuse,
                               const glm::vec3 &specular)
    : BaseLight(ambient, diffuse, specular), m_direction(direction)
{
}

DirectionLight::~DirectionLight()
{
}