
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
    for (auto input : inputs()) {
        if (input->type() == typeid(globjects::Shader *))
        {
            auto shaderInput = static_cast<Input<globjects::Shader *> *>(input);
            m_program->attach(**shaderInput);
        }
        if (input->type() == typeid(cppassist::FilePath))
        {
            auto fileInput = static_cast<Input<cppassist::FilePath> *>(input);
            auto shader = environment()->resourceManager()->load<globjects::Shader>((*fileInput)->path());
            // ToDo: Fix memory leak
            m_program->attach(shader);
        }
    }
    program.setValid(true);
}


} // namespace gloperate
