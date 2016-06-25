
#include <gloperate/pipeline/Pipeline.h>

#include <vector>
#include <set>

#include <cppassist/logging/logging.h>


using namespace cppassist;


namespace gloperate
{


// [TODO] invalidate sorting when stages or connections change


Pipeline::Pipeline(Environment * environment, const std::string & name)
: Stage(environment, name)
, m_sorted(false)
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

void Pipeline::addStage(Stage * stage, cppexpose::PropertyOwnership ownership)
{
    if (!stage)
    {
        return;
    }

    addProperty(stage, ownership);

    m_stages.push_back(stage);
    if (stage->name() != "") {
        m_stagesMap.insert(std::make_pair(stage->name(), stage));        
    }

    stageAdded(stage);
}

bool Pipeline::removeStage(Stage * stage)
{
    if (!stage)
    {
        return false;
    }

    auto it = std::find(m_stages.begin(), m_stages.end(), stage);
    if (it == m_stages.end())
    {
        return false;
    }

    m_stages.erase(it);
    m_stagesMap.erase(stage->name());

    stageRemoved(stage);

    removeProperty(stage);

    return true;
}

bool Pipeline::destroyStage(Stage * stage)
{
    if (!removeStage(stage))
    {
        return false;
    }

    delete stage;

    return true;
}

bool Pipeline::isPipeline() const
{
    return true;
}

void Pipeline::sortStages()
{
    auto couldBeSorted = true;
    std::vector<Stage *> sorted;
    std::set<Stage *> touched;

    std::function<void(Stage *)> visit = [&] (Stage * stage)
    {
        if (!couldBeSorted)
        {
            sorted.push_back(stage);
            return;
        }

        if (touched.count(stage) > 0)
        {
            critical() << "Pipeline is not a directed acyclic graph" << std::endl;
            couldBeSorted = false;
            sorted.push_back(stage);
            return;
        }

        touched.insert(stage);

        for (auto stageIt = m_stages.begin(); stageIt != m_stages.end(); /* nop */)
        {
            if (!stage->requires(*stageIt, false))
            {
                ++stageIt;
                continue;
            }

            auto nextStage = *stageIt;
            m_stages.erase(stageIt);
            visit(nextStage);

            stageIt = m_stages.begin();
        }

        sorted.push_back(stage);
    };

    while (!m_stages.empty())
    {
        auto stageIt = m_stages.begin();
        auto stage = *stageIt;
        m_stages.erase(stageIt);
        visit(stage);
    }

    m_stages = sorted;
    m_sorted = couldBeSorted;
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
    if (!m_sorted) {
        sortStages();
    }

    for (auto stage : m_stages)
    {
        if (stage->needsProcessing()) {
            stage->process(context);
        }
    }
}

void Pipeline::onInputValueChanged(AbstractSlot *)
{
    // Not necessary for pipelines (handled by inner connections)
}

void Pipeline::onOutputRequiredChanged(AbstractSlot *)
{
    // Not necessary for pipelines (handled by inner connections)
}


} // namespace gloperate
