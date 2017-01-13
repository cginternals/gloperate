
#include <gloperate/stages/base/ProgramStage.h>

#include <globjects/Shader.h>
#include <globjects/Program.h>

#include <gloperate/gloperate.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(ProgramStage, gloperate::Stage)


ProgramStage::ProgramStage(Environment * environment, const std::string & name)
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

ProgramStage::~ProgramStage()
{
}

void ProgramStage::onContextInit(AbstractGLContext *)
{
    m_program = new globjects::Program;
    program.setValue(m_program);
    program.setValid(false);
}

void ProgramStage::onProcess(AbstractGLContext *)
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
    program.setValid(true);
}


} // namespace gloperate
