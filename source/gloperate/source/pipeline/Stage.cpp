
#include <gloperate/pipeline/Stage.h>

#include <algorithm>

#include <cppassist/string/conversion.h>
#include <cppassist/logging/logging.h>

#include <cppexpose/variant/Variant.h>

#include <globjects/Texture.h>
#include <globjects/Framebuffer.h>

#include <gloperate/base/ExtendedProperties.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/pipeline/AbstractSlot.h>


using namespace cppassist;
using namespace cppexpose;


namespace gloperate
{


Stage::CreateConnectedInputProxy::CreateConnectedInputProxy(const std::string & name, Stage * stage)
: m_name(name)
, m_stage(stage)
, m_createdCount(0)
{
}

Stage::CreateConnectedInputProxy::~CreateConnectedInputProxy()
{
    assert(m_createdCount == 1);
}

Stage::Stage(Environment * environment, const std::string & className, const std::string & name)
: cppexpose::Object((name.empty()) ? className : name)
, m_environment(environment)
, m_alwaysProcess(false)
{
    // Set object class name
    setClassName(className);
}

Stage::~Stage()
{
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

    for (auto slot : m_inputs)
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
    debug(2, "gloperate") << this->qualifiedName() << ": initContext";
    onContextInit(context);
}

void Stage::deinitContext(AbstractGLContext * context)
{
    debug(2, "gloperate") << this->qualifiedName() << ": deinitContex";
    onContextDeinit(context);
}

void Stage::process()
{
    debug(1, "gloperate") << this->qualifiedName() << ": processing";
    onProcess();

    for (auto input : m_inputs)
    {
        input->setChanged(false);
    }
}

bool Stage::needsProcessing() const
{
    if (m_alwaysProcess) {
        debug(4, "gloperate") << this->qualifiedName() << ": needs processing because it is always processed";
        return true;
    }

    for (auto output : m_outputs)
    {
        if (output->isRequired() && !output->isValid()) {
            debug(4, "gloperate") << this->qualifiedName() << ": needs processing because output is invalid and required (" << output->qualifiedName()<< ")";
            return true;
        }
    }

    debug(4, "gloperate") << this->qualifiedName() << ": needs no processing";
    return false;
}

bool Stage::alwaysProcessed() const
{
    return m_alwaysProcess;
}

void Stage::setAlwaysProcessed(bool alwaysProcess)
{
    debug(2, "gloperate") << this->qualifiedName() << ": set always processed to " << alwaysProcess;
    m_alwaysProcess = alwaysProcess;
}

void Stage::invalidateOutputs()
{
    debug(3, "gloperate") << this->qualifiedName() << ": invalidateOutputs";

    for (auto output : m_outputs)
    {
        output->invalidate();
    }
}

AbstractSlot * Stage::getSlot(const std::string & path)
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

const std::vector<AbstractSlot *> & Stage::inputs() const
{
    return m_inputs;
}

const AbstractSlot * Stage::input(const std::string & name) const
{
    if (m_inputsMap.find(name) == m_inputsMap.end())
    {
        return nullptr;
    }

    return m_inputsMap.at(name);
}

AbstractSlot * Stage::input(const std::string & name)
{
    if (m_inputsMap.find(name) == m_inputsMap.end())
    {
        return nullptr;
    }

    return m_inputsMap.at(name);
}


Stage::CreateConnectedInputProxy Stage::createInput(const std::string & name)
{
    return { name, this };
}


void Stage::addInput(AbstractSlot * input)
{
    assert(input);

    // Find free name
    std::string name = getFreeName(input->name());
    input->setName(name);

    // Add input as property
    addProperty(input);

    // Add input
    registerInput(input);
}

void Stage::addInput(std::unique_ptr<AbstractSlot> && input)
{
    assert(input);

    // Find free name
    std::string name = getFreeName(input->name());
    input->setName(name);

    // Add input as property
    const auto inputPtr = input.get();
    addProperty(std::move(input));

    // Add input
    registerInput(inputPtr);
}

void Stage::registerInput(AbstractSlot * input)
{
    // Add input
    m_inputs.push_back(input);

    if (input->name() != "") {
        m_inputsMap.insert(std::make_pair(input->name(), input));
    }

    debug(2, "gloperate") << input->qualifiedName() << ": add input to stage";

    // Emit signal
    inputAdded(input);
}

void Stage::removeInput(AbstractSlot * input)
{
    // Find input
    auto it = std::find(m_inputs.begin(), m_inputs.end(), input);
    if (it != m_inputs.end())
    {
        debug(2, "gloperate") << input->qualifiedName() << ": remove input from stage";

        // Remove input
        m_inputs.erase(it);
        m_inputsMap.erase(input->name());

        // Emit signal
        inputRemoved(input);
    }

    // Remove property
    removeProperty(input);
}

const std::vector<AbstractSlot *> & Stage::outputs() const
{
    return m_outputs;
}

const AbstractSlot * Stage::output(const std::string & name) const
{
    if (m_outputsMap.find(name) == m_outputsMap.end())
    {
        return nullptr;
    }

    return m_outputsMap.at(name);
}

AbstractSlot * Stage::output(const std::string & name)
{
    if (m_outputsMap.find(name) == m_outputsMap.end())
    {
        return nullptr;
    }

    return m_outputsMap.at(name);
}

void Stage::addOutput(AbstractSlot * output)
{
    assert(output);

    // Find free name
    std::string name = getFreeName(output->name());
    output->setName(name);

    // Add output as property
    addProperty(output);

    // Add output
    registerOutput(output);
}

void Stage::addOutput(std::unique_ptr<AbstractSlot> && output)
{
    assert(output);

    // Find free name
    std::string name = getFreeName(output->name());
    output->setName(name);

    // Add output as property
    auto outputPtr = output.get();
    addProperty(std::move(output));

    // Add output
    registerOutput(outputPtr);
}

void Stage::registerOutput(AbstractSlot * output)
{
    // Add output
    m_outputs.push_back(output);
    if (output->name() != "") {
        m_outputsMap.insert(std::make_pair(output->name(), output));
    }

    debug(2, "gloperate") << output->qualifiedName() << ": add output to stage";

    // Emit signal
    outputAdded(output);
}

void Stage::removeOutput(AbstractSlot * output)
{
    // Find output
    auto it = std::find(m_outputs.begin(), m_outputs.end(), output);
    if (it != m_outputs.end())
    {
        debug(2, "gloperate") << output->qualifiedName() << ": remove output from stage";

        // Remove output
        m_outputs.erase(it);
        m_outputsMap.erase(output->name());

        // Emit signal
        outputRemoved(output);
    }

    // Remove property
    removeProperty(output);
}

void Stage::outputRequiredChanged(AbstractSlot * slot)
{
    debug(2, "gloperate") << this->qualifiedName() << ": output required changed for " << slot->qualifiedName();
    onOutputRequiredChanged(slot);
}

void Stage::inputValueChanged(AbstractSlot * slot)
{
    inputChanged(slot);

    onInputValueChanged(slot);
}

void Stage::inputValueInvalidated(AbstractSlot * slot)
{
    onInputValueInvalidated(slot);
}

void Stage::inputOptionsChanged(AbstractSlot * slot)
{
    inputChanged(slot);
}


std::string Stage::getFreeName(const std::string & name) const
{
    std::string nameOut = name;
    int i = 2;

    while (propertyExists(nameOut))
    {
        nameOut = name + cppassist::string::toString<int>(i);
        i++;
    }

    return nameOut;
}

AbstractSlot * Stage::createSlot(const std::string & slotType, const std::string & type, const std::string & name)
{
    if (type == "bool")    return createSlot<bool>                    (slotType, name);
    if (type == "int")     return createSlot<int>                     (slotType, name);
    if (type == "float")   return createSlot<float>                   (slotType, name);
    if (type == "vec2")    return createSlot<glm::vec2>               (slotType, name);
    if (type == "vec3")    return createSlot<glm::vec3>               (slotType, name);
    if (type == "vec4")    return createSlot<glm::vec4>               (slotType, name);
    if (type == "ivec2")   return createSlot<glm::ivec2>              (slotType, name);
    if (type == "ivec3")   return createSlot<glm::ivec3>              (slotType, name);
    if (type == "ivec4")   return createSlot<glm::ivec4>              (slotType, name);
    if (type == "string")  return createSlot<std::string>             (slotType, name);
    if (type == "file")    return createSlot<cppassist::FilePath>     (slotType, name);
    if (type == "color")   return createSlot<gloperate::Color>        (slotType, name);
    if (type == "texture") return createSlot<globjects::Texture *>    (slotType, name);
    if (type == "fbo")     return createSlot<globjects::Framebuffer *>(slotType, name);

    return nullptr;
}

std::string Stage::qualifiedName() const
{
    std::string path = name();

    Pipeline * pipeline = this->parentPipeline();
    while (pipeline)
    {
        path = pipeline->name() + "." + path;

        pipeline = pipeline->parentPipeline();
    }

    return path;
}

void Stage::onContextInit(AbstractGLContext *)
{
}

void Stage::onContextDeinit(AbstractGLContext *)
{
}

void Stage::onProcess()
{
}

void Stage::onInputValueChanged(AbstractSlot *)
{
    // Invalidate all outputs
    invalidateOutputs();
}

void Stage::onInputValueInvalidated(AbstractSlot *)
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

void Stage::invalidateInputConnections()
{
    if (parentPipeline())
    {
        parentPipeline()->invalidateStageOrder();
    }
}


} // namespace gloperate
