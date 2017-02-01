
#include <gloperate/stages/base/ProgramStage.h>

#include <cppassist/fs/FilePath.h>

#include <globjects/Shader.h>
#include <globjects/Program.h>

#include <gloperate/base/Environment.h>
#include <gloperate/base/ResourceManager.h>

#include <gloperate/gloperate.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(ProgramStage, gloperate::Stage)


ProgramStage::ProgramStage(Environment * environment, const std::string & name)
: Stage(environment, "ProgramStage", name)
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
    for (auto input : inputs<globjects::Shader *>()) {
        m_program->attach(input->value());
    }
    for (auto input : inputs<cppassist::FilePath>()) {
        auto shader = environment()->resourceManager()->load<globjects::Shader>((*input)->path());
        // ToDo: Fix memory leak
        m_program->attach(shader);
    }

    program.setValid(true);
}


} // namespace gloperate
