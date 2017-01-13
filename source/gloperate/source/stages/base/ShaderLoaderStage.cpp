
#include <gloperate/stages/base/ShaderLoaderStage.h>

#include <glbinding/gl/gl.h>

#include <globjects/Shader.h>

#include <gloperate/gloperate.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(ShaderLoaderStage, gloperate::Stage)


ShaderLoaderStage::ShaderLoaderStage(Environment * environment, const std::string & name)
: Stage(environment, name)
, filePath("filePath", this)
, source("source", this)
, type("type", this)
, m_file(nullptr)
, m_extensionToType({
    {"vert", gl::GL_VERTEX_SHADER},
    {"tesc", gl::GL_TESS_CONTROL_SHADER},
    {"tese", gl::GL_TESS_EVALUATION_SHADER},
    {"geom", gl::GL_GEOMETRY_SHADER},
    {"frag", gl::GL_FRAGMENT_SHADER},
    {"comp", gl::GL_COMPUTE_SHADER}})
{
}

ShaderLoaderStage::~ShaderLoaderStage()
{
}

void ShaderLoaderStage::onProcess(AbstractGLContext *)
{
    m_file = new globjects::File(filePath->path(), false);
    source.setValue(m_file);
    auto it = m_extensionToType.find(filePath->extension());
    if (it == m_extensionToType.end()) {
        type.setValue(gl::GL_NONE);
        return;
    }
    type.setValue((*it).second);
}


} // namespace gloperate
