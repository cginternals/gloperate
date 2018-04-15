
#include <gloperate/loaders/ShaderLoader.h>

#include <algorithm>

#include <cppassist/fs/readfile.h>
#include <cppassist/fs/FilePath.h>

#include <cppexpose/variant/Variant.h>

#include <glbinding-aux/Meta.h>

#include <globjects/base/File.h>
#include <globjects/Shader.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(ShaderLoader, gloperate::AbstractLoader)


ShaderLoader::ShaderLoader(Environment * environment)
: Loader<globjects::Shader>(environment)
, m_extensionToType({
    {"vert", gl::GL_VERTEX_SHADER},
    {"tesc", gl::GL_TESS_CONTROL_SHADER},
    {"tese", gl::GL_TESS_EVALUATION_SHADER},
    {"geom", gl::GL_GEOMETRY_SHADER},
    {"frag", gl::GL_FRAGMENT_SHADER},
    {"comp", gl::GL_COMPUTE_SHADER}})
{
    // Get list of supported file formats
    for (std::pair<std::string, gl::GLenum> kv_pair : m_extensionToType) {
        m_extensions.push_back(std::string(".") + kv_pair.first);
        m_types.push_back(glbinding::aux::Meta::getString(kv_pair.second) + " (*." + kv_pair.first + ")");
    }

    // Add entry that contains all supported file formats
    std::string allTypes;
    for (unsigned int i = 0; i < m_extensions.size(); ++i) {
        if (i > 0) allTypes += " ";
        allTypes += "*." + m_extensions[i].substr(1);
    }
    m_types.push_back(std::string("All shader formats (") + allTypes + ")");
}

ShaderLoader::~ShaderLoader()
{
}

bool ShaderLoader::canLoad(const std::string & ext) const
{
    // Check if file type is supported
    return (std::count(m_extensions.begin(), m_extensions.end(), "." + ext) > 0);
}

std::vector<std::string> ShaderLoader::loadingTypes() const
{
    // Return list of supported file types
    return m_types;
}

std::string ShaderLoader::allLoadingTypes() const
{
    // Compose list of all supported file extensions
    std::string allTypes;
    for (unsigned int i = 0; i < m_extensions.size(); ++i) {
        if (i > 0) allTypes += " ";
        allTypes += "*." + m_extensions[i].substr(1);
    }

    // Return supported types
    return allTypes;
}

globjects::Shader * ShaderLoader::load(const std::string & filename, const cppexpose::Variant &, std::function<void(int, int)> ) const
{
    globjects::Shader * shader;

    auto it = m_extensionToType.find(cppassist::FilePath(filename).extension());

    // [TODO] Remove file memory leak
    globjects::File * file = new globjects::File(filename, false);

    if (it == m_extensionToType.end() || file->string().empty()) {
        return nullptr;
    }

    shader = new globjects::Shader((*it).second, file);

    return shader;
}


} // namespace gloperate
