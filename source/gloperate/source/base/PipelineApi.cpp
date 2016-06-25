
#include <gloperate/base/PipelineApi.h>

#include <vector>
#include <sstream>
#include <string>

#include <gloperate/base/Environment.h>
#include <gloperate/base/Canvas.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/AbstractInputSlot.h>
#include <gloperate/pipeline/AbstractDataSlot.h>


namespace gloperate
{


PipelineApi::PipelineApi(Environment * environment)
: cppexpose::Object("pipeline")
, m_environment(environment)
{
    // Register functions
    addFunction("getName",         this, &PipelineApi::getName);
    addFunction("getStages",       this, &PipelineApi::getStages);
    addFunction("getInputs",       this, &PipelineApi::getInputs);
    addFunction("getParameters",   this, &PipelineApi::getParameters);
    addFunction("getOutputs",      this, &PipelineApi::getOutputs);
    addFunction("getProxyOutputs", this, &PipelineApi::getProxyOutputs);
    addFunction("getValue",        this, &PipelineApi::getValue);
    addFunction("setValue",        this, &PipelineApi::setValue);
    addFunction("isValid",         this, &PipelineApi::isValid);
    addFunction("isRequired",      this, &PipelineApi::isRequired);
    addFunction("setRequired",     this, &PipelineApi::setRequired);
}

PipelineApi::~PipelineApi()
{
}

std::string PipelineApi::getName(const std::string & name)
{
    Stage * stage = getStage(name);

    return stage->name();
}

cppexpose::Variant PipelineApi::getStages(const std::string & name)
{
    Stage * stage = getStage(name);

    cppexpose::Variant lst = cppexpose::Variant::array();

    if (!stage->isPipeline()) {
        return lst;
    }

    Pipeline * pipeline = static_cast<Pipeline*>(stage);

    for (auto * subStage : pipeline->stages())
    {
        lst.asArray()->push_back(subStage->name());
    }

    return lst;
}

cppexpose::Variant PipelineApi::getInputs(const std::string & name)
{
    Stage * stage = getStage(name);

    cppexpose::Variant lst = cppexpose::Variant::array();

    for (auto * input : stage->inputs())
    {
        lst.asArray()->push_back(input->name());
    }

    return lst;
}

cppexpose::Variant PipelineApi::getParameters(const std::string & name)
{
    Stage * stage = getStage(name);

    cppexpose::Variant lst = cppexpose::Variant::array();

    for (auto * parameter : stage->parameters())
    {
        lst.asArray()->push_back(parameter->name());
    }

    return lst;
}

cppexpose::Variant PipelineApi::getOutputs(const std::string & name)
{
    Stage * stage = getStage(name);

    cppexpose::Variant lst = cppexpose::Variant::array();

    for (auto * output : stage->outputs())
    {
        lst.asArray()->push_back(output->name());
    }

    return lst;
}

cppexpose::Variant PipelineApi::getProxyOutputs(const std::string & name)
{
    Stage * stage = getStage(name);

    cppexpose::Variant lst = cppexpose::Variant::array();

    for (auto * proxyOutput : stage->proxyOutputs())
    {
        lst.asArray()->push_back(proxyOutput->name());
    }

    return lst;
}

std::string PipelineApi::getValue(const std::string & path)
{
    AbstractSlot * slot = getSlot(path);
    if (slot) {
        return slot->toString();
    } else {
        return "";
    }
}

void PipelineApi::setValue(const std::string & path, const std::string & value)
{
    AbstractSlot * slot = getSlot(path);
    if (slot) {
        slot->fromString(value);
    }
}

bool PipelineApi::isValid(const std::string & path)
{
    AbstractSlot * slot = getSlot(path);
    if (slot) {
        return slot->isValid();
    } else {
        return false;
    }
}

bool PipelineApi::isRequired(const std::string & path)
{
    AbstractSlot * slot = getSlot(path);
    if (slot) {
        return slot->isRequired();
    } else {
        return false;
    }
}

void PipelineApi::setRequired(const std::string & path, bool required)
{
    AbstractSlot * slot = getSlot(path);
    if (slot) {
        return slot->setRequired(required);
    }
}

Stage * PipelineApi::getStage(const std::string & name)
{
    // Get canvas
    if (m_environment->canvases().size() == 0) {
        return nullptr;
    }

    Canvas * canvas = static_cast<Canvas *>(m_environment->canvases()[0]);
    if (!canvas) {
        return nullptr;
    }

    // Split name by '.'
    std::vector<std::string> names;
    std::istringstream ss(name);
    std::string subname;
    while (std::getline(ss, subname, '.')) {
        names.push_back(subname);
    }

    // Find stage in pipeline
    Stage * stage = nullptr;
    for (std::string subname : names)
    {
        // Get sub-stage
        if (!stage && subname == "Viewer") {
            stage = canvas->pipelineContainer();
        } else if (stage->isPipeline()) {
            stage = static_cast<Pipeline *>(stage)->stage(subname);
        } else {
            stage = nullptr;
        }

        // Abort if child was not found
        if (!stage) {
            return nullptr;
        }
    }

    return stage;
}

AbstractSlot * PipelineApi::getSlot(const std::string & name)
{
    // Get canvas
    if (m_environment->canvases().size() == 0) {
        return nullptr;
    }

    Canvas * canvas = static_cast<Canvas *>(m_environment->canvases()[0]);
    if (!canvas) {
        return nullptr;
    }

    // Split name by '.'
    std::vector<std::string> names;
    std::istringstream ss(name);
    std::string subname;
    while (std::getline(ss, subname, '.')) {
        names.push_back(subname);
    }

    // Find stage in pipeline
    Stage * stage = nullptr;
    for (size_t i=0; i<names.size(); i++)
    {
        // Get next token
        std::string subname = names[i];

        // If this is the last token, return property from current stage
        if (i == names.size() - 1)
        {
            AbstractProperty * property = stage->property(subname);
            AbstractSlot * slot = property ? static_cast<AbstractSlot *>(property) : nullptr;
            return slot;
        }

        // Get sub-stage
        if (!stage && subname == "Viewer") {
            stage = canvas->pipelineContainer();
        } else if (stage->isPipeline()) {
            stage = static_cast<Pipeline *>(stage)->stage(subname);
        } else {
            stage = nullptr;
        }

        // Abort if child was not found
        if (!stage) {
            return nullptr;
        }
    }

    return nullptr;
}


} // namespace gloperate
