
#include "Postprocessing.h"

#include <gloperate/painter/AbstractTargetFramebufferCapability.h>
#include <gloperate/painter/AbstractViewportCapability.h>
#include <gloperate/painter/AbstractVirtualTimeCapability.h>


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
