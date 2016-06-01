
#include <gloperate/pipeline/PipelineEvent.h>

#include <gloperate/pipeline/Pipeline.h>


namespace gloperate
{


PipelineEvent::PipelineEvent(Type type, Stage * stage)
: m_type(type)
, m_stage(stage)
, m_pipeline(nullptr)
, m_subStage(nullptr)
, m_slot(nullptr)
{
}

PipelineEvent::PipelineEvent(Type type, Stage * stage, AbstractSlot * slot)
: m_type(type)
, m_stage(stage)
, m_pipeline(nullptr)
, m_subStage(nullptr)
, m_slot(slot)
{
}

PipelineEvent::PipelineEvent(Type type, Pipeline * pipeline)
: m_type(type)
, m_stage(pipeline)
, m_pipeline(pipeline)
, m_subStage(nullptr)
, m_slot(nullptr)
{
}

PipelineEvent::PipelineEvent(Type type, Pipeline * pipeline, Stage * subStage)
: m_type(type)
, m_stage(pipeline)
, m_pipeline(pipeline)
, m_subStage(subStage)
, m_slot(nullptr)
{
}

PipelineEvent::~PipelineEvent()
{
}

PipelineEvent::Type PipelineEvent::type() const
{
    return m_type;
}

Stage * PipelineEvent::stage() const
{
    return m_stage;
}

Pipeline * PipelineEvent::pipeline() const
{
    return m_pipeline;
}

Stage * PipelineEvent::subStage() const
{
    return m_subStage;
}

AbstractSlot * PipelineEvent::slot() const
{
    return m_slot;
}


} // namespace gloperate
