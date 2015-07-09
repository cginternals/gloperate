
#include <gloperate/pipeline/PipelinePainter.h>

#include <gloperate/pipeline/AbstractPipeline.h>


namespace gloperate
{


PipelinePainter::PipelinePainter(
      const std::string & name
    , gloperate::ResourceManager & resourceManager
    , const std::string & relDataPath
    , AbstractPipeline & pipeline)
:   Painter(name, resourceManager, relDataPath)
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
