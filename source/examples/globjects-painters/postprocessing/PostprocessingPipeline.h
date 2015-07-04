
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

    
public:
    gloperate::Data<gloperate::AbstractTargetFramebufferCapability *> targetFBO;
    gloperate::Data<gloperate::AbstractViewportCapability *> viewport;
    gloperate::Data<gloperate::AbstractVirtualTimeCapability *> time;
    gloperate::Data<gloperate::AbstractCameraCapability *> camera;
    gloperate::Data<gloperate::AbstractProjectionCapability *> projection;
    gloperate::Data<gloperate::AbstractTypedRenderTargetCapability *> renderTargets;
};
