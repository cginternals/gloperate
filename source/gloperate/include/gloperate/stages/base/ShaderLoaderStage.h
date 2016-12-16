
#pragma once


#include <unordered_map>

#include <glm/glm.hpp>

#include <cppexpose/plugin/plugin_api.h>


#include <glbinding/gl/gl.h>

#include <globjects/base/ref_ptr.h>
#include <globjects/base/File.h>
#include <globjects/base/AbstractStringSource.h>

#include <gloperate/base/ExtendedProperties.h>
#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>


namespace globjects {
    class Shader;
    class AbstractStringSource;
}


namespace gloperate
{


/**
*  @brief
*    Stage that loads shader code from a file
*
*    Recognized file endings and inferred shader type:
*    *.vert - gl::GL_VERTEX_SHADER
*    *.tesc - gl::GL_TESS_CONTROL_SHADER
*    *.tese - gl::GL_TESS_EVALUATION_SHADER
*    *.geom - gl::GL_GEOMETRY_SHADER
*    *.frag - gl::GL_FRAGMENT_SHADER
*    *.comp - gl::GL_COMPUTE_SHADER
*/
class GLOPERATE_API ShaderLoaderStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        ShaderLoaderStage, gloperate::Stage
      , ""   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Stage that loads shader code from a file"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Inputs
    Input<cppassist::FilePath>                filePath;   ///< the file to load the shader from

    // Outputs
    Output<globjects::AbstractStringSource *> source;     ///< the shader source
    Output<gl::GLenum>                        type;       ///< inferred type from file ending


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment to which the stage belongs (must NOT be null!)
    *  @param[in] name
    *    Stage name
    */
    ShaderLoaderStage(Environment * environment, const std::string & name = "ShaderLoaderStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ShaderLoaderStage();


protected:
    // Virtual Stage interface
    virtual void onProcess(AbstractGLContext * context) override;

protected:
    globjects::ref_ptr<globjects::File> m_file;                          ///< File object
    const std::unordered_map<std::string, gl::GLenum> m_extensionToType; ///< Mapping of file extension to GLenum type
};


} // namespace gloperate
