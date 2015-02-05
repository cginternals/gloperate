#pragma once

#include <gloperate/pipeline/AbstractPipeline.h>
#include <gloperate/pipeline/Data.h>

namespace gloperate
{

class AbstractTargetFramebufferCapability;
class AbstractViewportCapability;
class AbstractVirtualTimeCapability;
class AbstractCameraCapability;
class AbstractProjectionCapability;
class AbstractTypedRenderTargetCapability;

}

class RasterizationStage;
class PostprocessingStage;

class PostprocessingPipeline : public gloperate::AbstractPipeline
{
public:
    PostprocessingPipeline();
    virtual ~PostprocessingPipeline();

    gloperate::AbstractTargetFramebufferCapability * targetFramebufferCapability();
    gloperate::AbstractViewportCapability * viewportCapability();
    gloperate::AbstractVirtualTimeCapability * virtualTimeCapability();
    gloperate::AbstractCameraCapability * cameraCapability();
    gloperate::AbstractProjectionCapability * projectionCapability();
    gloperate::AbstractTypedRenderTargetCapability * renderTargetCapability();

protected:
    RasterizationStage  * m_rasterization;
    PostprocessingStage * m_postprocessing;

    gloperate::Data<gloperate::AbstractTargetFramebufferCapability *> m_targetFBO;
    gloperate::Data<gloperate::AbstractViewportCapability *> m_viewport;
    gloperate::Data<gloperate::AbstractVirtualTimeCapability *> m_time;
    gloperate::Data<gloperate::AbstractCameraCapability *> m_camera;
    gloperate::Data<gloperate::AbstractProjectionCapability *> m_projection;
    gloperate::Data<gloperate::AbstractTypedRenderTargetCapability *> m_renderTargets;
};
