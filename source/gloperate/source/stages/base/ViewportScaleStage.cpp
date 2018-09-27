
#include <gloperate/stages/base/ViewportScaleStage.h>

#include <gloperate/rendering/AbstractRenderTarget.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(ViewportScaleStage, gloperate::Stage)


ViewportScaleStage::ViewportScaleStage(Environment * environment, const std::string & name)
: Stage(environment, "ViewportScaleStage", name)
, viewport      ("viewport",        this)
, scaleFactor   ("scaleFactor",     this)
, scaledViewport("scaledViewport", this)
{
}

ViewportScaleStage::~ViewportScaleStage()
{
}

void ViewportScaleStage::onProcess()
{

    const auto & viewport = this->viewport.value();
    const auto scaleFactor = this->scaleFactor.value();

    this->scaledViewport.setValue(glm::vec4(
        viewport.x,
        viewport.y,
        glm::max(scaleFactor * viewport.z, 1.0f),
        glm::max(scaleFactor * viewport.w, 1.0f)
    ));
}


} // namespace gloperate
