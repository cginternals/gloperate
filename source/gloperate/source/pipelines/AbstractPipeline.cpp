#include <cassert>

#include <string>
#include <gloperate/util/collection.hpp>

#include <algorithm>
#include <map>
#include <iostream>

#include <gloperate/pipelines/AbstractStage.h>
#include <gloperate/pipelines/AbstractPipeline.h>
#include <gloperate/pipelines/AbstractInputSlot.h>
#include <gloperate/pipelines/AbstractData.h>
#include <gloperate/pipelines/AbstractParameter.h>
#include <gloperate/pipelines/Parameter.h>

using namespace collection;

namespace gloperate
{

AbstractPipeline::AbstractPipeline()
: m_initialized(false)
, m_dependenciesSorted(false)
{
}

AbstractPipeline::~AbstractPipeline()
{
    for (AbstractStage* stage: m_stages)
	{
		delete stage;
	}
    for (AbstractParameter* parameter: m_parameters)
    {
        delete parameter;
    }
}

void AbstractPipeline::addStages()
{
}

void AbstractPipeline::addStage(AbstractStage* stage)
{
    m_stages.push_back(stage);
    stage->dependenciesChanged.connect([this]() { m_dependenciesSorted = false; });
}

void AbstractPipeline::addParameter(AbstractParameter * parameter)
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

void AbstractPipeline::addParameter(const std::string & name, AbstractParameter * parameter)
{
    parameter->setName(name);
    addParameter(parameter);
}

const std::vector<AbstractStage*>& AbstractPipeline::stages() const
{
    return m_stages;
}

const std::vector<AbstractParameter*> & AbstractPipeline::parameters() const
{
    return m_parameters;
}

std::vector<AbstractInputSlot*> AbstractPipeline::allInputs()
{
    return flatten(collect(m_stages, [](const AbstractStage * stage) { return stage->allInputs(); }));
}

std::vector<AbstractData*> AbstractPipeline::allOutputs()
{
    return flatten(collect(m_stages, [](const AbstractStage * stage) { return stage->allOutputs(); }));
}

AbstractParameter * AbstractPipeline::findParameter(const std::string & name)
{
    return detect(m_parameters, [&name](AbstractParameter * parameter) { return parameter->matchesName(name); }, nullptr);
}

std::vector<AbstractData *> AbstractPipeline::findOutputs(const std::string & name)
{
    return select(allOutputs(), [&name](AbstractData * data) { return data->matchesName(name); });
}

void AbstractPipeline::execute()
{
    if (!m_initialized)
    {
        return;
    }

    if (!m_dependenciesSorted)
    {
        sortDependencies();
    }
	
    for (AbstractStage* stage: m_stages)
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

    setupStages();
    setupParameters();
    connectStages();
    initializeStages();

    m_initialized = true;
}

void AbstractPipeline::setupParameters()
{
}

void AbstractPipeline::connectStages()
{
}

void AbstractPipeline::initializeStages()
{
    if (!m_dependenciesSorted)
    {
        sortDependencies();
    }

    for (AbstractStage * stage : m_stages)
    {
        stage->initialize();
    }
}

void AbstractPipeline::sortDependencies()
{
    if (m_dependenciesSorted)
        return;

    tsort(m_stages);
    m_dependenciesSorted = true;

    /*std::cout << "Sorted stages: ";
    for (AbstractStage* stage: m_stages)
    {
        std::cout << stage->asPrintable() << ", ";
    }
    std::cout << std::endl;*/
}

void AbstractPipeline::tsort(std::vector<AbstractStage*>& stages)
{
    std::vector<AbstractStage*> sorted(stages.size(), nullptr);
    auto iterator = sorted.rbegin();
    std::map<AbstractStage*, unsigned char> marks;

    std::function<void(AbstractStage*)> visit = [&](AbstractStage* stage)
    {
        if (marks[stage] == 1)
        {
            std::cerr << "Pipeline is not a directed acyclic graph" << std::endl;
            return;
        }

        if (marks[stage] == 0)
        {
            marks[stage] = 1;
            for (auto nextStage : stages)
            {
                if (nextStage->requires(stage, false))
                {
                    visit(nextStage);
                }
            }

            marks[stage] = 2;
            *iterator = stage;
            ++iterator;
        }
    };

    while (iterator != sorted.rend())
    {
        for (auto stage : stages)
        {
            if (marks[stage] == 0)
            {
                visit(stage);
            }
        }
    }

    stages.swap(sorted);
}

} // namespace gloperate
