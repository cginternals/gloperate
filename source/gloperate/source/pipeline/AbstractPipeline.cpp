#include <gloperate/pipeline/AbstractPipeline.h>

#include <cassert>
#include <string>
#include <algorithm>
#include <set>
#include <iostream>

#include <gloperate/base/collection.hpp>

#include <gloperate/pipeline/AbstractStage.h>
#include <gloperate/pipeline/AbstractInputSlot.h>
#include <gloperate/pipeline/AbstractData.h>
#include <gloperate/pipeline/AbstractData.h>
#include <gloperate/pipeline/Data.h>


using namespace collection;


namespace gloperate
{

AbstractPipeline::AbstractPipeline(const std::string & name)
:   m_initialized(false)
,   m_name(name)
,   m_dependenciesSorted(false)
{
}

AbstractPipeline::~AbstractPipeline()
{
    for (auto & stage : m_stages)
    {
        delete stage;
    }

    for (auto & constantParameter : m_constantParameters)
    {
        delete constantParameter;
    }
}

const std::string & AbstractPipeline::name() const
{
    return m_name;
}

void AbstractPipeline::setName(const std::string & name)
{
    m_name = name;
}

bool AbstractPipeline::hasName() const
{
    return !m_name.empty();
}

std::string AbstractPipeline::asPrintable() const
{
    if (!hasName())
        return "<unnamed>";

    std::string n = name();
    std::replace(n.begin(), n.end(), ' ', '_');

    return n;
}

void AbstractPipeline::addStage(AbstractStage * stage)
{
    stage->dependenciesChanged.connect([this]() { m_dependenciesSorted = false; });
    m_stages.push_back(stage);
}

void AbstractPipeline::addParameter(AbstractData * parameter)
{
    m_parameters.push_back(parameter);
}

void AbstractPipeline::shareData(const AbstractData* data)
{
    assert(data != nullptr);

    m_sharedData.push_back(data);
}

void AbstractPipeline::shareDataFrom(const AbstractInputSlot& slot)
{
    shareData(slot.connectedData());
}

void AbstractPipeline::addParameter(const std::string & name, AbstractData * parameter)
{
    parameter->setName(name);
    addParameter(parameter);
}

const std::vector<AbstractStage *> & AbstractPipeline::stages() const
{
    return m_stages;
}

const std::vector<AbstractData *> & AbstractPipeline::parameters() const
{
    return m_parameters;
}

std::vector<AbstractInputSlot *> AbstractPipeline::allInputs() const
{
    return flatten(collect(stages(), [](const AbstractStage * stage) { return stage->allInputs(); }));
}

std::vector<AbstractData *> AbstractPipeline::allOutputs() const
{
    return flatten(collect(stages(), [](const AbstractStage * stage) { return stage->allOutputs(); }));
}

AbstractData * AbstractPipeline::findParameter(const std::string & name) const
{
    return detect(m_parameters, [&name](AbstractData * parameter) { return parameter->matchesName(name); }, nullptr);
}

std::vector<AbstractData *> AbstractPipeline::findOutputs(const std::string & name) const
{
    return select(allOutputs(), [&name](AbstractData * data) { return data->matchesName(name); });
}

void AbstractPipeline::execute()
{
    if (!m_initialized)
    {
        return;
    }

    for (auto & stage: m_stages)
    {
        stage->execute();
    }
}

bool AbstractPipeline::isInitialized() const
{
    return m_initialized;
}

void AbstractPipeline::initialize()
{
    if (m_initialized)
        return;

    m_initialized = initializeStages();
}

bool AbstractPipeline::initializeStages()
{
    if (!m_dependenciesSorted)
    {
        if (!sortDependencies())
            return false;
    }

    for (auto & stage : m_stages)
    {
        stage->initialize();
    }

    return true;
}

bool AbstractPipeline::sortDependencies()
{
    if (m_dependenciesSorted)
        return true;

    m_dependenciesSorted = tsort(m_stages);
    return m_dependenciesSorted;
}

void AbstractPipeline::addStages()
{
}

bool AbstractPipeline::tsort(std::vector<AbstractStage *> & stages)
{
    auto couldBeSorted = true;
    std::vector<AbstractStage *> sorted;
    std::set<AbstractStage *> touched;

    std::function<void(AbstractStage *)> visit = [&](AbstractStage * stage)
    {
        if (!couldBeSorted)
        {
            sorted.push_back(stage);
            return;
        }

        if (touched.count(stage) > 0)
        {
            std::cerr << "Pipeline is not a directed acyclic graph" << std::endl;
            couldBeSorted = false;
            sorted.push_back(stage);
            return;
        }

        touched.insert(stage);

        for (auto stageIt = stages.begin(); stageIt != stages.end(); /* nop */)
        {
            if (!stage->requires(*stageIt, false))
            {
                ++stageIt;
                continue;
            }

            auto nextStage = *stageIt;
            stages.erase(stageIt);
            visit(nextStage);

            stageIt = stages.begin();
        }

        sorted.push_back(stage);
    };

    while (!stages.empty())
    {
        auto stageIt = stages.begin();
        auto stage = *stageIt;
        stages.erase(stageIt);
        visit(stage);
    }

    stages = sorted;

    return couldBeSorted;
}

} // namespace gloperate
