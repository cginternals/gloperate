
#include "Postprocessing.h"

#include <gloperate/painter/AbstractTargetFramebufferCapability.h>
#include <gloperate/painter/AbstractViewportCapability.h>
#include <gloperate/painter/AbstractVirtualTimeCapability.h>
#include <gloperate/painter/AbstractCameraCapability.h>
#include <gloperate/painter/AbstractProjectionCapability.h>
#include <gloperate/painter/AbstractTypedRenderTargetCapability.h>

Postprocessing::Postprocessing(gloperate::ResourceManager & resourceManager)
: PipelinePainter(resourceManager, m_pipeline)
{
    addCapability(m_pipeline.targetFramebufferCapability());
    addCapability(m_pipeline.viewportCapability());
    addCapability(m_pipeline.virtualTimeCapability());
    addCapability(m_pipeline.cameraCapability());
    addCapability(m_pipeline.projectionCapability());
    addCapability(m_pipeline.renderTargetCapability());
}

Postprocessing::~Postprocessing()
{
}
