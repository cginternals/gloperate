#pragma once


#include <gloperate/gloperate_api.h>

#include <gloperate/painter/Painter.h>


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
