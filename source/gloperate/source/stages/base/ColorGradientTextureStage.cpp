
#include <gloperate/stages/base/ColorGradientTextureStage.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(ColorGradientTextureStage, gloperate::Stage)


ColorGradientTextureStage::ColorGradientTextureStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, "ColorGradientTextureStage", name)
, gradients("gradients", this)
, textureWidth("textureWidth", this, 128)
, gradientTexture("gradientTexture", this)
{
}

ColorGradientTextureStage::~ColorGradientTextureStage()
{
}

void ColorGradientTextureStage::onContextInit(AbstractGLContext *)
{
}

void ColorGradientTextureStage::onContextDeinit(AbstractGLContext *)
{
    // Clean up OpenGL objects
    m_gradientTexture = nullptr;
}

void ColorGradientTextureStage::onProcess()
{
    // Generate texture
    m_gradientTexture = gradients->generateTexture(*textureWidth);

    // Update output
    this->gradientTexture.setValue(m_gradientTexture.get());
}


} // namespace gloperate
