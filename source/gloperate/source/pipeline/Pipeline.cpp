
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
    // Register functions
    addFunction("createStage",      this, &Pipeline::scr_createStage);
    addFunction("removeStage",      this, &Pipeline::scr_removeStage);
    addFunction("createConnection", this, &Pipeline::scr_createConnection);
    addFunction("removeConnection", this, &Pipeline::scr_removeConnection);
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
    if (m_stagesMap.find(name) == m_stagesMap.end())
    {
        return nullptr;
    }

    return m_stagesMap.at(name);
}

void Pipeline::addStage(Stage * stage, cppexpose::PropertyOwnership ownership)
{
    // Check parameters
    if (!stage)
    {
        return;
    }

    // Find free name
    std::string name = getFreeName(stage->name());
    stage->setName(name);

    // Add stage as property
    addProperty(stage, ownership);

    // Add stage
    m_stages.push_back(stage);
    if (stage->name() != "") {
        m_stagesMap.insert(std::make_pair(stage->name(), stage));        
    }

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

    stageRemoved(stage);

    removeProperty(stage);

    // [TODO]
    // Shouldn't be required if each slot of a stage would disconnect from connections
    // and this would be propagated to the normal stage order invalidation
    invalidateStageOrder();

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

void Pipeline::invalidateStageOrder()
{
    debug() << "Invalidate stage order; resort on next process";
    m_sorted = false;
}

AbstractSlot * Pipeline::getSlot(const std::string & path)
{
    std::vector<std::string> names = cppassist::string::split(path, '.');

    Stage * stage = this;

    for (size_t i=0; i<names.size(); i++)
    {
        std::string name = names[i];

        // Ignore own stage name at the beginning
        if (name == stage->name() && i == 0)
        {
            continue;
        }

        // Check if stage is a pipeline and has a substage with the given name
        if (stage->isPipeline())
        {
            Pipeline * pipeline = static_cast<Pipeline *>(stage);
            Stage * sub = pipeline->stage(name);

            if (sub)
            {
                stage = sub;
                continue;
            }
        }

        // If there is no more substage but more names to fetch, return error
        if (i != names.size() - 1)
        {
            return nullptr;
        }

        // Check if stage has a slot with that name
        auto inputSlot = stage->input(name);
        if (inputSlot) {
            return inputSlot;
        }

        auto outputSlot = stage->output(name);
        if (outputSlot) {
            return outputSlot;
        }
    }

    return nullptr;
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
            addSorted(stage);
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

cppexpose::Variant Pipeline::scr_getDescription()
{
    // Get stage description
    cppexpose::Variant obj = Stage::scr_getDescription();

    // List stages
    Variant stages = Variant::array();

    for (auto stage : m_stages)
    {
        stages.asArray()->push_back(stage->name());
    }

    (*obj.asMap())["stages"] = stages;

    // Return pipeline description
    return obj;
}

std::string Pipeline::scr_createStage(const std::string & className, const std::string & name)
{
    // Get component manager
    auto componentManager = m_environment->componentManager();

    // Get component for the requested stage
    auto component = componentManager->component<gloperate::Stage>(className);

    if (component)
    {
        // Create stage
        Stage * stage = component->createInstance(m_environment, name);
        addStage(stage);

        return stage->name();
    }

    return "";
}

void Pipeline::scr_removeStage(const std::string & name)
{
    Stage * stage = this->stage(name);

    removeStage(stage);
}

void Pipeline::scr_createConnection(const std::string & from, const std::string & to)
{
    AbstractSlot * slotTo   = getSlot(to);
    AbstractSlot * slotFrom = getSlot(from);

    if (slotTo && slotFrom)
    {
        slotTo->connect(slotFrom);
    }
}

void Pipeline::scr_removeConnection(const std::string & to)
{
    AbstractSlot * slotTo = getSlot(to);

    if (slotTo)
    {
        slotTo->disconnect();
    }
}


} // namespace gloperate
