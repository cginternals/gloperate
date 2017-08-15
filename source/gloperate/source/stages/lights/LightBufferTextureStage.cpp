
#include <gloperate/stages/lights/LightBufferTextureStage.h>

#include <glm/vec3.hpp>

#include <glbinding/gl/enum.h>

#include <globjects/Buffer.h>
#include <globjects/Texture.h>

#include <gloperate/rendering/Light.h>


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

CPPEXPOSE_COMPONENT(LightBufferTextureStage, gloperate::Stage)


LightBufferTextureStage::LightBufferTextureStage(Environment * environment, const std::string & name)
: Stage(environment, "LightBufferTextureStage", name)
, colorTypeData("colorTypeData", this, nullptr)
, positionData("positionData", this, nullptr)
, attenuationData("attenuationData", this, nullptr)
{
}

LightBufferTextureStage::~LightBufferTextureStage()
{
}

void LightBufferTextureStage::onContextInit(AbstractGLContext * /*context*/)
{
    setupBufferTextures();
}

void LightBufferTextureStage::onContextDeinit(AbstractGLContext * /*context*/)
{
    m_attenuationTexture.reset();
    m_positionTexture.reset();
    m_colorTypeTexture.reset();

    m_attenuationBuffer.reset();
    m_positionBuffer.reset();
    m_colorTypeBuffer.reset();
}

void LightBufferTextureStage::onProcess()
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

    colorTypeData.setValue(m_colorTypeTexture.get());
    positionData.setValue(m_positionTexture.get());
    attenuationData.setValue(m_attenuationTexture.get());
}

void LightBufferTextureStage::setupBufferTextures()
{
    m_colorTypeBuffer = cppassist::make_unique<globjects::Buffer>();
    m_colorTypeTexture = cppassist::make_unique<globjects::Texture>(gl::GL_TEXTURE_BUFFER);

    m_positionBuffer = cppassist::make_unique<globjects::Buffer>();
    m_positionTexture = cppassist::make_unique<globjects::Texture>(gl::GL_TEXTURE_BUFFER);

    m_attenuationBuffer = cppassist::make_unique<globjects::Buffer>();
    m_attenuationTexture = cppassist::make_unique<globjects::Texture>(gl::GL_TEXTURE_BUFFER);

    m_colorTypeTexture->texBuffer(gl::GL_RGBA32F, m_colorTypeBuffer.get());
    m_positionTexture->texBuffer(gl::GL_RGB32F, m_positionBuffer.get());
    m_attenuationTexture->texBuffer(gl::GL_RGB32F, m_attenuationBuffer.get());
}

Input<Light> * LightBufferTextureStage::createLightInput()
{
    auto lightInput = createInput<Light>("light");
    m_lightInputs.push_back(lightInput);
    return lightInput;
}


} // namespace gloperate
