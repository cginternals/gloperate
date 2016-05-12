
#include <gloperate/pipeline/Stage.h>

#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/pipeline/InputSlot.h>
#include <gloperate/pipeline/Data.h>


namespace gloperate
{


Stage::Stage(ViewerContext * viewerContext, Pipeline * parent)
: cppexpose::Object("", parent)
, m_viewerContext(viewerContext)
, m_parentPipeline(parent)
{
}

Stage::~Stage()
{
}

ViewerContext * Stage::viewerContext() const
{
    return m_viewerContext;
}

Pipeline * Stage::parentPipeline() const
{
    return m_parentPipeline;
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

const std::vector<AbstractInputSlot *> & Stage::inputs() const
{
    return m_inputs;
}

const AbstractInputSlot * Stage::input(const std::string & name) const
{
    return m_inputsMap.at(name);
}

const std::vector<AbstractData *> & Stage::outputs() const
{
    return m_outputs;
}

const AbstractData * Stage::output(const std::string & name) const
{
    return m_outputsMap.at(name);
}

void Stage::registerInput(AbstractInputSlot * input)
{
    // Check parameters
    if (!input) {
        return;
    }

    // Add input
    m_inputs.push_back(input);
    if (input->name() != "") {
        m_inputsMap.insert(std::make_pair(input->name(), input));        
    }
}

void Stage::registerOutput(AbstractData * output)
{
    // Check parameters
    if (!output) {
        return;
    }

    // Add output
    m_outputs.push_back(output);
    if (output->name() != "") {
        m_outputsMap.insert(std::make_pair(output->name(), output));        
    }
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


} // namespace gloperate
