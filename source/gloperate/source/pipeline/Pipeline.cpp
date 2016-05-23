
#include <gloperate/pipeline/Pipeline.h>

#include <gloperate/pipeline/PipelineEvent.h>


namespace gloperate
{


Pipeline::Pipeline(ViewerContext * viewerContext, const std::string & name, Pipeline * parent)
: Stage(viewerContext, name, parent)
{
}

Pipeline::~Pipeline()
{
}

const std::vector<Stage *> Pipeline::stages() const
{
    return m_stages;
}

Stage * Pipeline::stage(const std::string & name) const
{
    return m_stagesMap.at(name);
}

bool Pipeline::isPipeline() const
{
    return true;
}

void Pipeline::registerStage(Stage * stage)
{
    if (!stage)
    {
        return;
    }

    m_stages.push_back(stage);
    if (stage->name() != "") {
        m_stagesMap.insert(std::make_pair(stage->name(), stage));        
    }

    stageAdded(stage);

    promotePipelineEvent(
        PipelineEvent(PipelineEvent::StageAdded, this, stage)
    );
}

void Pipeline::unregisterStage(Stage * stage)
{
    if (!stage)
    {
        return;
    }

    auto it = std::find(m_stages.begin(), m_stages.end(), stage);
    if (it != m_stages.end())
    {
        m_stages.erase(it);
        m_stagesMap.erase(stage->name());

        stageRemoved(stage);

        promotePipelineEvent(
            PipelineEvent(PipelineEvent::StageRemoved, this, stage)
        );
    }
}

void Pipeline::onContextInit(AbstractGLContext * context)
{
    for (auto stage : m_stages)
    {
        stage->initContext(context);
    }
}

void Pipeline::onContextDeinit(AbstractGLContext * context)
{
    for (auto stage : m_stages)
    {
        stage->deinitContext(context);
    }
}

void Pipeline::onProcess(AbstractGLContext * context)
{
    for (auto stage : m_stages)
    {
        stage->process(context);
    }
}

void Pipeline::onInputValueChanged(AbstractSlot *)
{
    // The pipeline does not need to react on input value changes
    // as they should be connected to stages inside the pipeline
}


} // namespace gloperate
