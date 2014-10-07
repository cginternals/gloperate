#include <gloperate/pipelines/PipelinePainter.h>

#include <gloperate/pipelines/AbstractPipeline.h>

namespace gloperate
{

PipelinePainter::PipelinePainter(gloperate::ResourceManager & resourceManager, AbstractPipeline & pipeline)
: Painter(resourceManager)
, m_pipeline(pipeline)
{
}

PipelinePainter::~PipelinePainter()
{
}

void PipelinePainter::onInitialize()
{
    m_pipeline.initialize();
}

void PipelinePainter::onPaint()
{
    m_pipeline.execute();
}

} // namespace gloperate
