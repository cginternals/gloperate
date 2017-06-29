
#include <gloperate/stages/base/ColorGradientStage.h>

#include <gloperate/base/Environment.h>
#include <gloperate/base/ResourceManager.h>
#include <gloperate/rendering/ColorGradientList.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(ColorGradientStage, gloperate::Stage)


ColorGradientStage::ColorGradientStage(Environment * environment, const std::string & name)
: Stage(environment, "ColorGradientStage", name)
, filePath("filePath", this)
, gradients("gradients", this)
{
}

ColorGradientStage::~ColorGradientStage()
{
}

void ColorGradientStage::onContextInit(AbstractGLContext *)
{
}

void ColorGradientStage::onContextDeinit(AbstractGLContext *)
{
    // Clean up OpenGL objects
    m_colorGradientList = nullptr;
}

void ColorGradientStage::onProcess()
{
    // Load color gradient list
    m_colorGradientList = std::unique_ptr<ColorGradientList>(environment()->resourceManager()->load<ColorGradientList>((*filePath).path()));

    // Update outputs
    gradients.setValue(m_colorGradientList.get());
}


} // namespace gloperate
