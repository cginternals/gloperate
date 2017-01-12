
#include <gloperate/stages/base/RenderPassStage.h>

#include <globjects/Shader.h>
#include <globjects/Program.h>

#include <gloperate/gloperate.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(RenderPassStage, gloperate::Stage)


RenderPassStage::RenderPassStage(Environment * environment, const std::string & name)
: Stage(environment, name)
, program("program", this)
{
    inputAdded.connect([this] (gloperate::AbstractSlot * /*addedInput*/) {
        program.setValid(false);
    });
    inputRemoved.connect([this] (gloperate::AbstractSlot * /*addedInput*/) {
        program.setValid(false);
    });
}

RenderPassStage::~RenderPassStage()
{
}

void RenderPassStage::onContextInit(AbstractGLContext *)
{
    m_program = new globjects::Program;
}

void RenderPassStage::onProcess(AbstractGLContext *)
{
    for (auto shader : m_program->shaders()) {
        m_program->detach(shader);
    }
    for (auto input : inputs()) {
        auto shaderInput = dynamic_cast<Input<globjects::Shader *> *>(input);
        if (shaderInput)
        {
            m_program->attach(**shaderInput);
        }
    }
    program.setValue(m_program);
}


} // namespace gloperate
