
#include <gloperate/stages/base/ShaderStage.h>

#include <globjects/Shader.h>

#include <gloperate/gloperate.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(ShaderStage, gloperate::Stage)


ShaderStage::ShaderStage(Environment * environment, const std::string & name)
: Stage(environment, name)
, type("type", this)
, source("source", this)
, shader("shader", this)
{
}

ShaderStage::~ShaderStage()
{
}

void ShaderStage::onProcess(AbstractGLContext *)
{
    m_shader = new globjects::Shader(*type, *source);
    shader.setValue(m_shader);
}


} // namespace gloperate
