
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
    // Invalidate output when input slots have been added or removed
    inputAdded.connect([this] (gloperate::AbstractSlot * )
    {
        program.invalidate();
    });

    inputRemoved.connect([this] (gloperate::AbstractSlot *)
    {
        program.invalidate();
    });
}

ProgramStage::~ProgramStage()
{
}

void ProgramStage::onContextInit(AbstractGLContext *)
{
    m_program = cppassist::make_unique<globjects::Program>();
}

void ProgramStage::onContextDeinit(AbstractGLContext *)
{
    // Clean up OpenGL objects
    m_shaders.clear();
    m_program = nullptr;
}

void ProgramStage::onProcess()
{
    // Detach all shaders from program
    for (auto shader : m_program->shaders())
    {
        m_program->detach(shader);
    }

    // Attach all shaders from inputs of type Shader
    for (auto input : inputs<globjects::Shader *>())
    {
        if (input)
        {
            m_program->attach(input->value());
        }
    }

    // Load and attach all shaders from inputs of type FilePath
    for (auto input : inputs<cppassist::FilePath>())
    {
        if (auto shader = environment()->resourceManager()->load<globjects::Shader>((*input)->path()))
        {
            m_shaders.emplace_back(shader);
            m_program->attach(shader);
        }
    }

    // Update output
    program.setValue(m_program.get());
}


} // namespace gloperate
