#pragma once

#include <gloperate/pipelines/AbstractPipeline.h>
#include <gloperate/pipelines/Data.h>

#include <pipeline-examples/pipeline_examples_api.h>

namespace gloperate
{

class AbstractTargetFramebufferCapability;
class AbstractViewportCapability;
class AbstractVirtualTimeCapability;
class Camera;

}

class RasterizationStage;
class PostprocessingStage;

class PIPELINE_EXAMPLES_API PostprocessingPipeline : public gloperate::AbstractPipeline
{
public:
    PostprocessingPipeline();
    virtual ~PostprocessingPipeline();

    gloperate::AbstractTargetFramebufferCapability * targetFramebufferCapability();
    gloperate::AbstractViewportCapability * viewportCapability();
    gloperate::AbstractVirtualTimeCapability * virtualTimeCapability();

protected:
    RasterizationStage  * m_rasterization;
    PostprocessingStage * m_postprocessing;

    gloperate::Data<gloperate::Camera * > m_camera;
    gloperate::Data<gloperate::AbstractTargetFramebufferCapability *> m_targetFBO;
    gloperate::Data<gloperate::AbstractViewportCapability *> m_viewport;
    gloperate::Data<gloperate::AbstractVirtualTimeCapability *> m_time;
};
