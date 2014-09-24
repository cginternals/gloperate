#pragma once

#include <gloperate/Painter.h>

#include <gloperate/gloperate_api.h>

namespace gloperate
{

class ResourceManager;
class AbstractPipeline;

class GLOPERATE_API PipelinePainter : public gloperate::Painter
{
public:
    PipelinePainter(gloperate::ResourceManager & resourceManager, AbstractPipeline & pipeline);
    virtual ~PipelinePainter();

    virtual void onInitialize() override;
    virtual void onPaint() override;
protected:
    AbstractPipeline & m_pipeline;
};

} // namespace gloperate
