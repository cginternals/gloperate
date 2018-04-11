
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
    std::vector<ColorGradientList *> gradientLists;

    for (auto input : inputs())
    {
        if (input->type() == typeid(ColorGradientList *))
        {
            gradientLists.push_back(static_cast<Input<ColorGradientList *> *>(input)->value());
        }
    }

    m_gradientTexture = ColorGradientList::generateTexture(gradientLists, *textureWidth);

    // Update output
    this->texture.setValue(m_gradientTexture.get());
}


} // namespace gloperate
