
#pragma once


#include <string>

#include <reflectionzeug/base/Color.h>

#include <globjects/base/ref_ptr.h>
#include <globjects/Texture.h>

#include <gloperate/pipeline/AbstractPipeline.h>
#include <gloperate/pipeline/Data.h>
#include <gloperate/base/ColorGradientList.h>


namespace cpplocate
{
    class ModuleInfo;
}

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
    PostprocessingPipeline(const cpplocate::ModuleInfo & moduleInfo);
    virtual ~PostprocessingPipeline();

    
public:
    gloperate::Data<gloperate::AbstractTargetFramebufferCapability *> targetFBO;
    gloperate::Data<gloperate::AbstractViewportCapability *> viewport;
    gloperate::Data<gloperate::AbstractVirtualTimeCapability *> time;
    gloperate::Data<gloperate::AbstractCameraCapability *> camera;
    gloperate::Data<gloperate::AbstractProjectionCapability *> projection;
    gloperate::Data<gloperate::AbstractTypedRenderTargetCapability *> renderTargets;

    gloperate::Data<size_t> gradientsTextureWidth;
    gloperate::Data<std::string> gradientName;
    gloperate::Data<gloperate::ColorGradientList> gradients;

    std::string m_dataPath;
};
