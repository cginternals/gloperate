#pragma once

#include <gloperate/Painter.h>

namespace gloperate
{

class ResourceManager;
class AbstractPipeline;

class PipelinePainter : public gloperate::Painter
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
