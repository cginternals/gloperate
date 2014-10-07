#include <pipeline-examples/Postprocessing/Postprocessing.h>

#include <gloperate/capabilities/AbstractTargetFramebufferCapability.h>
#include <gloperate/capabilities/AbstractViewportCapability.h>
#include <gloperate/capabilities/AbstractVirtualTimeCapability.h>

Postprocessing::Postprocessing(gloperate::ResourceManager & resourceManager)
: PipelinePainter(resourceManager, m_pipeline)
{
    addCapability(m_pipeline.targetFramebufferCapability());
    addCapability(m_pipeline.viewportCapability());
    addCapability(m_pipeline.virtualTimeCapability());
}

Postprocessing::~Postprocessing()
{
}
