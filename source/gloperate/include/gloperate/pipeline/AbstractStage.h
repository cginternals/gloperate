#pragma once

#include <set>
#include <string>

#include <gloperate/gloperate_api.h>

#include <globjects/base/CachedValue.h>
#include <signalzeug/Signal.h>


namespace gloperate
{

class AbstractInputSlot;
class AbstractData;


class GLOPERATE_API AbstractStage
{
public:
    AbstractStage(const std::string & name = "");
    virtual ~AbstractStage();

    const std::string & name() const;
    void setName(const std::string & name);

    bool hasName() const;

    std::string asPrintable() const;

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
    void addDependency(AbstractStage * stage);

    void alwaysProcess(bool on);
    bool isAlwaysProcess() const;
    void scheduleProcess();

    void invalidateOutputs();

public:
    signalzeug::Signal<> dependenciesChanged;

protected:
    bool needsToProcess() const;
    bool inputsUsable() const;
    void markInputsProcessed();

    virtual void process() = 0;

protected:
    bool m_enabled;
    bool m_alwaysProcess;
    bool m_processScheduled;
    std::string m_name;
    globjects::CachedValue<bool> m_usable;

    std::set<AbstractData*> m_outputs;
    std::set<AbstractData*> m_sharedOutputs;
    std::set<AbstractInputSlot*> m_inputs;
    std::set<AbstractInputSlot*> m_sharedInputs;
    std::set<AbstractStage*> m_dependencies;    /**< Additional manual dependencies not expressed by data connections */

private:
    AbstractStage(const AbstractStage&) = delete;
};

} // namespace gloperate
