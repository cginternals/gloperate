
#include <gloperate/stages/base/ColorGradientTextureStage.h>

#include <globjects/Texture.h>

#include <gloperate/rendering/ColorGradientList.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(ColorGradientTextureStage, gloperate::Stage)


ColorGradientTextureStage::ColorGradientTextureStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, "ColorGradientTextureStage", name)
, gradients("gradients", this)
, textureWidth("textureWidth", this, 128)
, texture("texture", this)
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
    std::vector<ColorGradientList *> additionalGradients;

    for (auto input : inputs())
    {
        // Only consider dynamic inputs
        if (!input->isDynamic())
            continue;

        if (input->type() == typeid(ColorGradientList *))
        {
            additionalGradients.push_back(static_cast<Input<ColorGradientList *> *>(input)->value());
        }
    }

    if (additionalGradients.size() == 0)
    {
        // Generate texture
        m_gradientTexture = gradients->generateTexture(*textureWidth);
    } else {
        m_gradientTexture = gradients->generateCompositeTexture(*textureWidth, additionalGradients);
    }

    // Update output
    this->texture.setValue(m_gradientTexture.get());
}


} // namespace gloperate
