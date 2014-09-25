
#include <iostream>
#include <algorithm>

#include <gloperate/pipelines/AbstractInputSlot.h>
#include <gloperate/pipelines/AbstractData.h>

#include <gloperate/pipelines/AbstractStage.h>

namespace gloperate
{

AbstractStage::AbstractStage(const std::string & name)
: Nameable(name)
, m_enabled(true)
, m_alwaysProcess(false)
{
    dependenciesChanged.connect([this]() { m_usable.invalidate(); });
}

AbstractStage::~AbstractStage()
{
}

bool AbstractStage::execute()
{
    if (!m_enabled)
        return false;

    if (!inputsUsable())
        return false;

    if (needsToProcess())
        process();
    else
        return false;
    
    markInputsProcessed();
    
    return true;
}

void AbstractStage::initialize()
{
}

bool AbstractStage::needsToProcess() const
{
    std::set<AbstractInputSlot*> inputs = allInputs();
    return m_alwaysProcess || inputs.empty() || std::any_of(inputs.begin(), inputs.end(), [](const AbstractInputSlot * input) {
        return input->hasChanged();
    });
}

bool AbstractStage::inputsUsable() const
{
    if (m_usable.isValid())
        return m_usable.value();

    std::set<AbstractInputSlot*> inputs = allInputs();
    m_usable.setValue(std::all_of(inputs.begin(), inputs.end(), [](const AbstractInputSlot * input) {
        return input->isUsable();
    }));

    if (!m_usable.value())
    {
        std::cout << "Some inputs in " << asPrintable() << " are not be connected: ";
        for (AbstractInputSlot * slot : inputs)
            if (!slot->isUsable())
                std::cout << slot->asPrintable() << ". ";
        std::cout << std::endl;
    }

    return m_usable.value();
}

void AbstractStage::markInputsProcessed()
{
    for (AbstractInputSlot * input : m_inputs)
    {
        input->processed();
    }
}

void AbstractStage::invalidateOutputs()
{
    for (AbstractData * output : m_outputs)
    {
        output->invalidate();
    }
}

void AbstractStage::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

bool AbstractStage::isEnabled() const
{
    return m_enabled;
}

void AbstractStage::alwaysProcess(bool on)
{
    m_alwaysProcess = on;
}

bool AbstractStage::requires(const AbstractStage * stage, bool recursive) const
{
    for (AbstractInputSlot * slot : m_inputs)
    {
        if (slot->isFeedback() || !slot->connectedData() || !slot->connectedData()->owner())
            continue;

        if (slot->connectedData()->owner() == stage || (recursive && slot->connectedData()->owner()->requires(stage)))
            return true;
    }

    return false;
}

const std::set<AbstractData*> & AbstractStage::outputs() const
{
    return m_outputs;
}

std::set<AbstractData*> AbstractStage::allOutputs() const
{
    std::set<AbstractData*> outputs;

    std::set_union(
        m_outputs.begin(),
        m_outputs.end(),
        m_sharedOutputs.begin(),
        m_sharedOutputs.end(),
        std::inserter(outputs, outputs.end())
    );

    return outputs;
}

const std::set<AbstractInputSlot*> & AbstractStage::inputs() const
{
    return m_inputs;
}

std::set<AbstractInputSlot*> AbstractStage::allInputs() const
{
    std::set<AbstractInputSlot*> inputs;

    std::set_union(
        m_inputs.begin(),
        m_inputs.end(),
        m_sharedInputs.begin(),
        m_sharedInputs.end(),
        std::inserter(inputs, inputs.end())
    );

    return inputs;
}

void AbstractStage::addOutput(const std::string & name, AbstractData & output)
{
    output.setName(name);
    output.setOwner(this);
    m_outputs.insert(&output);
}

void AbstractStage::shareOutput(AbstractData* output)
{
    m_sharedOutputs.insert(output);
}

void AbstractStage::addInput(const std::string & name, AbstractInputSlot & input)
{
    input.setName(name);
    input.setOwner(this);
    m_inputs.insert(&input);

    input.connectionChanged.connect(dependenciesChanged);
}

void AbstractStage::shareInput(AbstractInputSlot * input)
{
    m_sharedInputs.insert(input);

    input->connectionChanged.connect(dependenciesChanged);
}

void AbstractStage::addFeedbackInput(const std::string & name, AbstractInputSlot & input)
{
    input.setFeedback(true);
    addInput(name, input);
}

void AbstractStage::addOptionalInput(const std::string & name, AbstractInputSlot & input)
{
    input.setOptional(true);
    addInput(name, input);
}

} // namespace gloperate
