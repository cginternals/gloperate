
#include <gloperate/stages/lights/LightDataAccumulationStage.h>

#include <glm/vec3.hpp>

#include <glbinding/gl/enum.h>

#include <globjects/Buffer.h>
#include <globjects/Texture.h>

#include <gloperate/rendering/LightDefinition.h>


namespace
{
struct ColorTypeEntry
{
    glm::vec3 color;
    float type;
};
}


namespace gloperate
{

CPPEXPOSE_COMPONENT(LightDataAccumulationStage, gloperate::Stage)


LightDataAccumulationStage::LightDataAccumulationStage(Environment * environment, const std::string & name)
: Stage(environment, "LightDataAccumulationStage", name)
, colorTypeData("colorTypeData", this, nullptr)
, positionData("positionData", this, nullptr)
, attenuationData("attenuationData", this, nullptr)
{
}

LightDataAccumulationStage::~LightDataAccumulationStage()
{
}

void LightDataAccumulationStage::onContextInit(AbstractGLContext * context)
{
}

void LightDataAccumulationStage::onProcess(AbstractGLContext * context)
{
    if (!colorTypeData.isValid() || !positionData.isValid() || !attenuationData.isValid())
    {
        setupBufferTextures();
    }

    std::vector<ColorTypeEntry> colorsTypes{};
    std::vector<glm::vec3> positions{};
    std::vector<glm::vec3> attenuations{};

    for (auto lightInput : m_lightInputs)
    {
        if (!lightInput->isValid())
            continue;

        auto lightDef = lightInput->value();
        colorsTypes.push_back(ColorTypeEntry{lightDef.color, float(lightDef.type)});
        positions.push_back(lightDef.position);
        attenuations.push_back(lightDef.attenuationCoefficients);
    }

    m_colorTypeBuffer->setData(colorsTypes, gl::GL_DYNAMIC_DRAW);
    m_positionBuffer->setData(positions, gl::GL_DYNAMIC_DRAW);
    m_attenuationBuffer->setData(attenuations, gl::GL_DYNAMIC_DRAW);

    colorTypeData.setValue(m_colorTypeTexture);
    positionData.setValue(m_positionTexture);
    attenuationData.setValue(m_attenuationTexture);
}

void LightDataAccumulationStage::setupBufferTextures()
{
    m_colorTypeBuffer = new globjects::Buffer();
    m_colorTypeTexture = new globjects::Texture(gl::GL_TEXTURE_BUFFER);

    m_positionBuffer = new globjects::Buffer();
    m_positionTexture = new globjects::Texture(gl::GL_TEXTURE_BUFFER);

    m_attenuationBuffer = new globjects::Buffer();
    m_attenuationTexture = new globjects::Texture(gl::GL_TEXTURE_BUFFER);

    m_colorTypeTexture->texBuffer(gl::GL_RGBA32F, m_colorTypeBuffer);
    m_positionTexture->texBuffer(gl::GL_RGB32F, m_positionBuffer);
    m_attenuationTexture->texBuffer(gl::GL_RGB32F, m_attenuationBuffer);
}

Input<LightDefinition> * LightDataAccumulationStage::createLightInput()
{
    auto lightInput = createInput<LightDefinition>("light");
    m_lightInputs.push_back(lightInput);
    return lightInput;
}


} // namespace gloperate
