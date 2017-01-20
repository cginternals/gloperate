
#include <gloperate/stages/base/ShaderStage.h>

#include <globjects/Shader.h>

#include <gloperate/base/Environment.h>
#include <gloperate/base/ResourceManager.h>

#include <gloperate/gloperate.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(ShaderStage, gloperate::Stage)


ShaderStage::ShaderStage(Environment * environment, const std::string & name)
: Stage(environment, name)
, filePath("filePath", this)
, shader("shader", this)
{
}

ShaderStage::~ShaderStage()
{
}

void ShaderStage::onProcess(AbstractGLContext *)
{
    m_shader = environment()->resourceManager()->load<globjects::Shader>((*filePath).path());
    shader.setValue(m_shader);
}


} // namespace gloperate
