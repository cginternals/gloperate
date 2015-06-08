#include <gloperate/pipeline/PipelinePainter.h>

#include <gloperate/pipeline/AbstractPipeline.h>


namespace gloperate
{

PipelinePainter::PipelinePainter(
    gloperate::ResourceManager & resourceManager, 
    AbstractPipeline & pipeline, 
    const std::string & name)
:   Painter(resourceManager, name)
,   m_pipeline(pipeline)
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

reflectionzeug::AbstractProperty * PipelinePainter::propertyFor(gloperate::AbstractData * parameter) const
{
    auto it = m_dataToPropertyMap.find(parameter);
    if (it == m_dataToPropertyMap.end())
        return nullptr;

    return it->second;
}

reflectionzeug::AbstractProperty * PipelinePainter::property(const std::string & name) const
{
    return propertyFor(m_pipeline.findParameter(name));
}

} // namespace gloperate
