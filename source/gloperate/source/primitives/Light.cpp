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
