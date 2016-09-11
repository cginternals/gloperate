
#include <gloperate/pipeline/Stage.h>

#include <algorithm>

#include <cppassist/logging/logging.h>

#include <cppexpose/variant/Variant.h>

#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/pipeline/AbstractInputSlot.h>
#include <gloperate/pipeline/AbstractDataSlot.h>


using namespace cppassist;
using namespace cppexpose;


namespace gloperate
{


Stage::Stage(Environment * environment, const std::string & name)
: cppexpose::Object(name)
, m_environment(environment)
, m_alwaysProcess(false)
{
    // Register functions
    addFunction("getDescription", this, &Stage::scr_getDescription);
    addFunction("getConnections", this, &Stage::scr_getConnections);
}

Stage::~Stage()
{
    info() << m_name << " destroyed.";

    if (Pipeline * parent = parentPipeline())
    {
        parent->removeStage(this);
    }
}

bool Stage::isPipeline() const
{
    return false;
}

Environment * Stage::environment() const
{
    return m_environment;
}

Pipeline * Stage::parentPipeline() const
{
    return dynamic_cast<Pipeline *>(m_parent);
}

bool Stage::requires(const Stage * stage, bool recursive) const
{
    if (!stage)
    {
        return false;
    }

    for (AbstractInputSlot * slot : m_inputs)
    {
        if (slot->isFeedback() || !slot->isConnected())
            continue;

        if (slot->source()->parentStage() == stage || (recursive && slot->source()->parentStage()->requires(stage)))
            return true;
    }

    return false;
}

void Stage::initContext(AbstractGLContext * context)
{
    onContextInit(context);
}

void Stage::deinitContext(AbstractGLContext * context)
{
    onContextDeinit(context);
}

void Stage::process(AbstractGLContext * context)
{
    onProcess(context);
}

bool Stage::needsProcessing() const
{
    if (m_alwaysProcess) {
        return true;
    }

    for (auto output : m_outputs) {
        if (output->isRequired() && !output->isValid()) {
            return true;
        }
    }

    for (auto output : m_proxyOutputs) {
        if (output->isRequired() && !output->isValid()) {
            return true;
        }
    }

    return false;
}

bool Stage::alwaysProcessed() const
{
    return m_alwaysProcess;
}

void Stage::setAlwaysProcessed(bool alwaysProcess)
{
    m_alwaysProcess = alwaysProcess;
}

void Stage::invalidateOutputs()
{
    for (auto output : m_outputs)
    {
        output->setValid(false);
    }
}

const std::vector<AbstractInputSlot *> & Stage::inputs() const
{
    return m_inputs;
}

const AbstractInputSlot * Stage::input(const std::string & name) const
{
    return m_inputsMap.at(name);
}

void Stage::addInput(AbstractInputSlot * input, cppexpose::PropertyOwnership ownership)
{
    // Check parameters
    if (!input) {
        return;
    }

    // Add input as property
    addProperty(input, ownership);

    // Add input
    m_inputs.push_back(input);

    if (input->name() != "") {
        m_inputsMap.insert(std::make_pair(input->name(), input));        
    }

    // Emit signal
    inputAdded(input);
}

void Stage::removeInput(AbstractInputSlot * input)
{
    // Check parameters
    if (!input)
    {
        return;
    }

    // Find input
    auto it = std::find(m_inputs.begin(), m_inputs.end(), input);
    if (it != m_inputs.end())
    {
        // Remove input
        m_inputs.erase(it);
        m_inputsMap.erase(input->name());

        // Emit signal
        inputRemoved(input);
    }

    // Remove property
    removeProperty(input);
}

const std::vector<AbstractDataSlot *> & Stage::parameters() const
{
    return m_parameters;
}

const AbstractDataSlot * Stage::parameter(const std::string & name) const
{
    return m_parametersMap.at(name);
}

void Stage::addParameter(AbstractDataSlot * parameter, cppexpose::PropertyOwnership ownership)
{
    // Check parameters
    if (!parameter) {
        return;
    }

    // Add parameter as property
    addProperty(parameter, ownership);

    // Add parameter
    m_parameters.push_back(parameter);
    if (parameter->name() != "") {
        m_parametersMap.insert(std::make_pair(parameter->name(), parameter));        
    }

    // Emit signal
    parameterAdded(parameter);
}

void Stage::removeParameter(AbstractDataSlot * parameter)
{
    // Check parameters
    if (!parameter)
    {
        return;
    }

    // Find parameter
    auto it = std::find(m_parameters.begin(), m_parameters.end(), parameter);
    if (it != m_parameters.end())
    {
        // Remove parameter
        m_parameters.erase(it);
        m_parametersMap.erase(parameter->name());

        // Emit signal
        parameterRemoved(parameter);
    }

    // Remove property
    removeProperty(parameter);
}

const std::vector<AbstractDataSlot *> & Stage::outputs() const
{
    return m_outputs;
}

const AbstractDataSlot * Stage::output(const std::string & name) const
{
    return m_outputsMap.at(name);
}

void Stage::addOutput(AbstractDataSlot * output, cppexpose::PropertyOwnership ownership)
{
    // Check parameters
    if (!output) {
        return;
    }

    // Add output as property
    addProperty(output, ownership);

    // Add output
    m_outputs.push_back(output);
    if (output->name() != "") {
        m_outputsMap.insert(std::make_pair(output->name(), output));        
    }

    // Emit signal
    outputAdded(output);
}

void Stage::removeOutput(AbstractDataSlot * output)
{
    // Check parameters
    if (!output)
    {
        return;
    }

    // Find output
    auto it = std::find(m_outputs.begin(), m_outputs.end(), output);
    if (it != m_outputs.end())
    {
        // Remove output
        m_outputs.erase(it);
        m_outputsMap.erase(output->name());

        // Emit signal
        outputRemoved(output);
    }

    // Remove property
    removeProperty(output);
}

const std::vector<AbstractInputSlot *> & Stage::proxyOutputs() const
{
    return m_proxyOutputs;
}

const AbstractInputSlot * Stage::proxyOutput(const std::string & name) const
{
    return m_proxyOutputsMap.at(name);
}

void Stage::addProxyOutput(AbstractInputSlot * proxyOutput, cppexpose::PropertyOwnership ownership)
{
    // Check parameters
    if (!proxyOutput) {
        return;
    }

    // Add proxy output as property
    addProperty(proxyOutput, ownership);

    // Add proxy output
    m_proxyOutputs.push_back(proxyOutput);
    if (proxyOutput->name() != "") {
        m_proxyOutputsMap.insert(std::make_pair(proxyOutput->name(), proxyOutput));        
    }

    // Emit signal
    proxyOutputAdded(proxyOutput);
}

void Stage::removeProxyOutput(AbstractInputSlot * proxyOutput)
{
    // Check parameters
    if (!proxyOutput)
    {
        return;
    }

    // Find proxy output
    auto it = std::find(m_proxyOutputs.begin(), m_proxyOutputs.end(), proxyOutput);
    if (it != m_proxyOutputs.end())
    {
        // Remove proxy output
        m_proxyOutputs.erase(it);
        m_proxyOutputsMap.erase(proxyOutput->name());

        // Emit signal
        proxyOutputRemoved(proxyOutput);
    }

    // Remove property
    removeProperty(proxyOutput);
}

void Stage::outputRequiredChanged(AbstractSlot *slot)
{
    onOutputRequiredChanged(slot);
}

void Stage::inputValueChanged(AbstractSlot *slot)
{
    onInputValueChanged(slot);
}

void Stage::onContextInit(AbstractGLContext *)
{
}

void Stage::onContextDeinit(AbstractGLContext *)
{
}

void Stage::onProcess(AbstractGLContext *)
{
}

void Stage::onInputValueChanged(AbstractSlot *)
{
    // Invalidate all outputs
    invalidateOutputs();
}

void Stage::onOutputRequiredChanged(AbstractSlot *)
{
    // By default, assume nothing is required
    bool required = false;

    // Check if any output is required
    for (auto output : m_outputs)
    {
        if (output->isRequired()) {
            required = true;
            break;
        }
    }

    // Update all inputs
    for (auto input : m_inputs)
    {
        input->setRequired(required);
    }
}

cppexpose::Variant Stage::scr_getDescription()
{
    Variant obj = Variant::map();

    // Get name
    (*obj.asMap())["name"] = name();

    // List parameters
    Variant parameters = Variant::array();

    for (auto param : m_parameters)
    {
        parameters.asArray()->push_back(param->name());
    }

    (*obj.asMap())["parameters"] = parameters;

    // List inputs
    Variant inputs = Variant::array();

    for (auto input : m_inputs)
    {
        inputs.asArray()->push_back(input->name());
    }

    (*obj.asMap())["inputs"] = inputs;

    // List outputs
    Variant outputs = Variant::array();

    for (auto output : m_outputs)
    {
        outputs.asArray()->push_back(output->name());
    }

    (*obj.asMap())["outputs"] = outputs;

    // List proxy outputs
    Variant proxyOutputs = Variant::array();

    for (auto proxy : m_proxyOutputs)
    {
        proxyOutputs.asArray()->push_back(proxy->name());
    }

    (*obj.asMap())["proxyOutputs"] = proxyOutputs;

    // Return stage description
    return obj;
}

cppexpose::Variant Stage::scr_getConnections()
{
    Variant obj = Variant::array();

    auto addInputSlot = [&obj, this] (AbstractInputSlot * slot)
    {
        // Check if slot is connected
        if (slot->isConnected())
        {
            // Get connection info
            std::string from = slot->source()->qualifiedName();
            if (from.substr(0, 9) == "pipeline.") from = from.substr(9);

            std::string to = slot->qualifiedName();
            if (to.substr(0, 9) == "pipeline.") to = to.substr(9);

            // Describe connection
            Variant connection = Variant::map();
            (*connection.asMap())["from"] = from;
            (*connection.asMap())["to"]   = to;

            // Add connection
            obj.asArray()->push_back(connection);
        }
    };

    // List connections
    for (auto input : m_inputs)
    {
        addInputSlot(input);
    }

    for (auto proxy : m_proxyOutputs)
    {
        addInputSlot(proxy);
    }

    // Return connections
    return obj;
}


} // namespace gloperate
