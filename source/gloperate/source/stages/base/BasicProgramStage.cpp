
#include <gloperate/stages/base/BasicProgramStage.h>

#include <cppassist/logging/logging.h>

#include <gloperate/stages/base/ProgramStage.h>
#include <gloperate/stages/base/ShaderLoaderStage.h>
#include <gloperate/stages/base/ShaderStage.h>

#include <gloperate/gloperate.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(BasicProgramStage, gloperate::Stage)


const std::string BasicProgramStage::s_loaderStagePrefix = "ls_";
const std::string BasicProgramStage::s_shaderStagePrefix = "s_";

BasicProgramStage::BasicProgramStage(Environment * environment, const std::string & name)
: Pipeline(environment, name)
, program("program", this)
, m_programStage(new ProgramStage(environment, "ProgramStage"))
{
    inputAdded.connect([this] (gloperate::AbstractSlot * addedInput) {
        this->onInputAdded(addedInput);
    });
    inputRemoved.connect([this] (gloperate::AbstractSlot * removedInput) {
        this->onInputRemoved(removedInput);
    });

    addStage(m_programStage);

    // connect output
    program << m_programStage->program;
}

BasicProgramStage::~BasicProgramStage()
{
}

void BasicProgramStage::onInputAdded(gloperate::AbstractSlot * addedInput)
{
    auto shaderInput = dynamic_cast<Input<cppassist::FilePath> *>(addedInput);
    if (!shaderInput)
    {
        cppassist::warning() << "Input ignored, this stage only accepts cppassist::FilePath inputs.";
        return;
    }

    ShaderLoaderStage * shaderLoaderStage = new ShaderLoaderStage(environment(), s_loaderStagePrefix + addedInput->name());
    ShaderStage * shaderStage = new ShaderStage(environment(), s_shaderStagePrefix + addedInput->name());

    addStage(shaderLoaderStage);
    addStage(shaderStage);

    shaderLoaderStage->filePath << *shaderInput;

    shaderStage->type << shaderLoaderStage->type;
    shaderStage->source << shaderLoaderStage->source;

    *(m_programStage->createInput<globjects::Shader *>(addedInput->name())) << shaderStage->shader;
}

void BasicProgramStage::onInputRemoved(gloperate::AbstractSlot * removedInput)
{
    gloperate::Stage * shaderLoaderStage = m_stagesMap[s_loaderStagePrefix + removedInput->name()];
    gloperate::Stage * shaderStage = m_stagesMap[s_shaderStagePrefix + removedInput->name()];

    destroyStage(shaderLoaderStage);
    destroyStage(shaderStage);

    delete m_programStage->input(removedInput->name());
}


} // namespace gloperate
