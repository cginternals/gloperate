#include <gloperate/primitives/Light.h>

namespace gloperate {

Light::Light()
    : m_type (LightSourceType::Undefined)
    , m_attenuationConstant(0.f)
    , m_attenuationLinear(1.f)
    , m_attenuationQuadratic(0.f)
    , m_cosineCutoff(0.f)
{
}

Light::Light(const Light & other)
    : m_name(other.m_name)
    , m_type(other.m_type)
    , m_position(other.m_position)
    , m_direction(other.m_direction)
    , m_attenuationConstant(other.m_attenuationConstant)
    , m_attenuationLinear(other.m_attenuationLinear)
    , m_attenuationQuadratic(other.m_attenuationQuadratic)
    , m_colorDiffuse( other.m_colorDiffuse)
    , m_colorSpecular( other.m_colorSpecular)
    , m_cosineCutoff(other.m_cosineCutoff)
{
}

LightSourceType Light::type() const
{
    return m_type;
}

std::string Light::name() const
{
    return m_name;
}

glm::vec3 Light::position() const
{
    return m_position;
}

glm::vec3 Light::direction() const
{
    return m_direction;
}

glm::vec3 Light::colorDiffuse() const
{
    return m_colorDiffuse;
}

glm::vec3 Light::colorSpecular() const
{
    return m_colorSpecular;
}

float Light::attenuationConstant() const
{
    return m_attenuationConstant;
}

float Light::attenuationLinear() const
{
    return m_attenuationLinear;
}

float Light::attenuationQuadratic() const
{
    return m_attenuationQuadratic;
}

float Light::cosineCutoff() const
{
    return m_cosineCutoff;
}

void Light::setType(LightSourceType type)
{
    m_type = type;
}

void Light::setName(std::string name)
{
    m_name = name;
}

void Light::setPosition(glm::vec3 position)
{
    m_position = position;
}

void Light::setDirection(glm::vec3 direction)
{
    m_direction = direction;
}

void Light::setColorDiffuse(glm::vec3 colorDiffuse)
{
    m_colorDiffuse = colorDiffuse;
}

void Light::setColorSpecular(glm::vec3 colorSpecular)
{
    m_colorSpecular = colorSpecular;
}

void Light::setAttenuationLinear(float attenuationLinear)
{
    m_attenuationLinear = attenuationLinear;
}

void Light::setAttenuationConstant(float attenuationConstant)
{
    m_attenuationConstant = attenuationConstant;
}

void Light::setAttenuationQuadratic(float attenuationQuadratic)
{
    m_attenuationQuadratic = attenuationQuadratic;
}

void Light::setCosineCutoff(float cosineCutoff)
{
    m_cosineCutoff = cosineCutoff;
}

} // namespace gloperate
