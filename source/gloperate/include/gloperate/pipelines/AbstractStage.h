#pragma once

#include <set>
#include <string>

#include <gloperate/signals/Signal.h>

#include <gloperate/gloperate_api.h>

#include <gloperate/pipelines/Nameable.h>

namespace gloperate
{

class AbstractInputSlot;
class AbstractData;

class GLOPERATE_API AbstractStage : public Nameable
{
    friend class AbstractData;
public:
    //AbstractStage();
    AbstractStage(const std::string & name);
	virtual ~AbstractStage();

    bool execute();

    virtual void initialize();

    void setEnabled(bool enabled);
    bool isEnabled() const;

    bool requires(const AbstractStage * stage, bool recursive = true) const;

    const std::set<AbstractData*> & outputs() const;
    std::set<AbstractData*> allOutputs() const;
    const std::set<AbstractInputSlot*> & inputs() const;
    std::set<AbstractInputSlot*> allInputs() const;

    void addOutput(const std::string & name, AbstractData & output);
    void shareOutput(AbstractData * output);
    void addInput(const std::string & name, AbstractInputSlot & input);
    void shareInput(AbstractInputSlot * input);
    void addFeedbackInput(const std::string & name, AbstractInputSlot & input);
    void addOptionalInput(const std::string & name, AbstractInputSlot & input);

    void alwaysProcess(bool on);

    void invalidateOutputs();
public:
    gloperate::Signal<> dependenciesChanged;

protected:
    bool needsToProcess() const;
    bool inputsUsable() const;
    void markInputsProcessed();

    virtual void process() = 0;

protected:
    bool m_enabled;
    bool m_alwaysProcess;
    mutable bool m_usable;
    mutable bool m_usableValid;

    std::set<AbstractData*> m_outputs;
    std::set<AbstractData*> m_sharedOutputs;
    std::set<AbstractInputSlot*> m_inputs;
    std::set<AbstractInputSlot*> m_sharedInputs;

private:
    AbstractStage(const AbstractStage&) = delete;
};

} // namespace gloperate
