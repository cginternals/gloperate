
#include <gloperate/stages/base/VirtualViewportStage.h>

#include <gloperate/rendering/AbstractRenderTarget.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(VirtualViewportStage, gloperate::Stage)


VirtualViewportStage::VirtualViewportStage(Environment * environment, const std::string & name)
: Stage(environment, "VirtualViewportStage", name)
, viewport       ("viewport",        this)
, scaleFactor    ("scaleFactor",     this)
, virtualViewport("virtualViewport", this)
{
}

VirtualViewportStage::~VirtualViewportStage()
{
}

void VirtualViewportStage::onProcess()
{

    m_virtualViewport = this->viewport.value() * this->scaleFactor.value();

    this->virtualViewport.setValue(m_virtualViewport);
}


} // namespace gloperate
