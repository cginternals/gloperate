
#include <gloperate/pipeline/Pipeline.h>

#include <iostream>
#include <vector>
#include <set>

#include <cppassist/logging/logging.h>
#include <cppassist/string/manipulation.h>

#include <cppexpose/variant/Variant.h>

#include <gloperate/base/Environment.h>
#include <gloperate/base/ComponentManager.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>


using namespace cppassist;
using namespace cppexpose;


namespace gloperate
{


// [TODO] invalidate sorting when stages or connections change


Pipeline::Pipeline(Environment * environment, const std::string & className, const std::string & name)
: Stage(environment, className, name)
, m_sorted(false)
{
}

Pipeline::~Pipeline()
{
    // Do not use iterators here, as removeStage modifies m_stages
    while (!m_stages.empty())
    {
        removeStage(m_stages.back());
    }
}

const std::vector<Stage *> & Pipeline::stages() const
{
    return m_stages;
}

Stage * Pipeline::stage(const std::string & name) const
{
    if (m_stagesMap.find(name) == m_stagesMap.end())
    {
        return nullptr;
    }

    return m_stagesMap.at(name);
}

void Pipeline::addStage(Stage * stage)
{
    assert(stage);

    // Find free name
    std::string name = getFreeName(stage->name());
    stage->setName(name);

    // Add stage as property
    addProperty(stage);

    // Add stage
    registerStage(stage);
}

void Pipeline::addStage(std::unique_ptr<Stage> && stage)
{
    assert(stage);

    // Find free name
    std::string name = getFreeName(stage->name());
    stage->setName(name);

    // Add stage as property
    const auto stagePtr = stage.get();
    addProperty(std::move(stage));

    // Add stage
    registerStage(stagePtr);
}

void Pipeline::registerStage(Stage * stage)
{
    // Add stage
    m_stages.push_back(stage);
    if (stage->name() != "") {
        m_stagesMap.insert(std::make_pair(stage->name(), stage));
    }

    debug(1, "gloperate") << stage->qualifiedName() << ": add to pipeline";

    // Shouldn't be required if each slot of a stage would disconnect from connections
    // and this would be propagated to the normal stage order invalidation
    invalidateStageOrder();

    // Emit signal
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

    debug(1, "gloperate") << stage->qualifiedName() << ": remove from pipeline";

    stageRemoved(stage);

    removeProperty(stage);

    // [TODO]
    // Shouldn't be required if each slot of a stage would disconnect from connections
    // and this would be propagated to the normal stage order invalidation
    invalidateStageOrder();

    return true;
}

void Pipeline::invalidateStageOrder()
{
    debug(1, "gloperate") << this->name() << ": invalidate stage order; resort on next process";
    m_sorted = false;
}

bool Pipeline::isPipeline() const
{
    return true;
}

void Pipeline::sortStages()
{
    debug("gloperate") << this->qualifiedName() << ": sort stages";

    auto couldBeSorted = true;
    std::vector<Stage *> sorted;
    std::set<Stage *> touched;

    std::function<void(Stage *)> addSorted = [&](Stage * stage)
    {
        m_stages.erase(find(m_stages.begin(), m_stages.end(), stage));
        sorted.push_back(stage);
    };

    std::function<void(Stage *)> visit = [&](Stage * stage)
    {
        if (!couldBeSorted)
        {
            addSorted(stage);
            return;
        }

        if (touched.count(stage) > 0)
        {
            critical() << "Pipeline is not a directed acyclic graph";
            couldBeSorted = false;
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
            visit(nextStage);

            if (!couldBeSorted)
            {
                addSorted(stage);
                return;
            }

            stageIt = m_stages.begin();
        }

        addSorted(stage);
    };

    while (!m_stages.empty())
    {
        auto stageIt = m_stages.begin();
        auto stage = *stageIt;
        visit(stage);
    }

    debug(2, "gloperate") << "Stage order after sorting";
    for (const auto stage : sorted)
    {
        debug(2, "gloperate") << stage->qualifiedName();
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

void Pipeline::onProcess()
{
    if (!m_sorted) {
        sortStages();
    }

    for (auto stage : m_stages)
    {
        if (stage->needsProcessing()) {
            stage->process();
        }
        else
        {
            debug(2, "gloperate") << stage->qualifiedName() << ": omit execution";
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
