#include <gloperate/primitives/Light.h>
namespace gloperate {

Light::Light()
    :	m_type                 (UNDIFINED)
    ,	m_attenuationConstant  (0.f)
    ,   m_attenuationLinear    (1.f)
    ,   m_attenuationQuadratic (0.f)
    ,   m_cosinusCutOff        (0.f)
{
}

Light::Light(const Light & toCopy)
{
    m_type = toCopy.type();
    m_name = toCopy.name();
    m_position = toCopy.position();
    m_direction = toCopy.direction();
    m_colorDiffuse = toCopy.colorDiffuse();
    m_colorSpecular = toCopy.colorSpecular();
    m_attenuationConstant = toCopy.attenuationConst();
    m_attenuationLinear = toCopy.attenuationLinear();
    m_attenuationQuadratic = toCopy.attenuationQuad();
    m_cosinusCutOff = toCopy.cosinusCutOff();
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

float Light::attenuationConst() const
{
    return m_attenuationConstant;
}

float Light::attenuationLinear() const
{
    return m_attenuationLinear;
}

float Light::attenuationQuad() const
{
    return m_attenuationQuadratic;
}

float Light::cosinusCutOff() const
{
    return m_cosinusCutOff;
}

void Light::type(int type){
    m_type = LightSourceType(type);
}

void Light::name(std::string name)
{
    m_name = name;
}

void Light::position(glm::vec3 pos){
    m_position = pos;
}

void Light::direction(glm::vec3 dir)
{
    m_direction = dir;
}

void Light::colorDiffuse(glm::vec3 colorDiff)
{
    m_colorDiffuse = colorDiff;
}

void Light::colorSpecular(glm::vec3 colorSpec)
{
    m_colorSpecular = colorSpec;
}

void Light::attenuationLinear(float attLin)
{
    m_attenuationLinear = attLin;
}

void Light::attenuationConst(float attConst)
{
    m_attenuationConstant = attConst;
}

void Light::attenuationQuad(float attQuad)
{
    m_attenuationQuadratic = attQuad;
}

void Light::cosinusCutOff(float cosCutOff)
{
    m_cosinusCutOff = cosCutOff;
}

} // namespace gloperate
