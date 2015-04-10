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

AbstractPipeline::~AbstractPipeline() = default;

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

void AbstractPipeline::addStage(std::unique_ptr<AbstractStage> stage)
{
    stage->dependenciesChanged.connect([this]() { m_dependenciesSorted = false; });
    m_stages.push_back(std::move(stage));
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

std::vector<AbstractStage *> AbstractPipeline::stages() const
{
    return collect(m_stages, [](const std::unique_ptr<AbstractStage> & stage) { return stage.get(); });
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

bool AbstractPipeline::tsort(std::vector<std::unique_ptr<AbstractStage>> & stages)
{
    auto couldBeSorted = true;
    std::vector<std::unique_ptr<AbstractStage>> sorted;
    std::set<AbstractStage *> touched;

    std::function<void(std::unique_ptr<AbstractStage>)> visit = [&](std::unique_ptr<AbstractStage> stage)
    {
        if (!couldBeSorted)
        {
            sorted.push_back(std::move(stage));
            return;
        }

        if (touched.count(stage.get()) > 0)
        {
            std::cerr << "Pipeline is not a directed acyclic graph" << std::endl;
            couldBeSorted = false;
            sorted.push_back(std::move(stage));
            return;
        }

        touched.insert(stage.get());

        for (auto stageIt = stages.begin(); stageIt != stages.end();)
        {
            if (!stage->requires((*stageIt).get(), false))
            {
                ++stageIt;
                continue;
            }

            auto nextStage = std::move(*stageIt);
            stageIt = stages.erase(stageIt);
            visit(std::move(nextStage));
        }

        sorted.push_back(std::move(stage));
    };

    while (!stages.empty())
    {
        auto stageIt = stages.begin();
        auto stage = std::move(*stageIt);
        stages.erase(stageIt);
        visit(std::move(stage));
    }

    stages = std::move(sorted);
    return couldBeSorted;
}

} // namespace gloperate
