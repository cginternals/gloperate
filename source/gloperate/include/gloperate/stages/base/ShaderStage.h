
#pragma once


#include <glm/glm.hpp>

#include <cppassist/fs/FilePath.h>

#include <cppexpose/plugin/plugin_api.h>

#include <glbinding/gl/gl.h>

#include <globjects/base/ref_ptr.h>

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
*    Stage that loads and creates a shader from a file path
*/
class GLOPERATE_API ShaderStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        ShaderStage, gloperate::Stage
      , ""   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Stage that loads and creates a shader from a file path"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Inputs
    Input<cppassist::FilePath>   filePath;   ///< path to the source file of the shader

    // Outputs
    Output<globjects::Shader *>  shader;     ///< the shader object


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
    ShaderStage(Environment * environment, const std::string & name = "ShaderStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ShaderStage();


protected:
    // Virtual Stage interface
    virtual void onProcess(AbstractGLContext * context) override;

protected:
    globjects::ref_ptr<globjects::Shader> m_shader; ///< Shader object
};


} // namespace gloperate
