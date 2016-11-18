
#pragma once


#include <glm/glm.hpp>

#include <cppexpose/plugin/plugin_api.h>

#include <glbinding/gl/gl.h>

#include <globjects/base/ref_ptr.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>


namespace globjects {
    class Program;
    class Shader;
}

namespace cppassist {
    class FilePath;
}

namespace gloperate
{


/**
*  @brief
*    Stage that creates a shader from shader code
*/
class GLOPERATE_API ProgramStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        ProgramStage, gloperate::Stage
      , ""   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Stage that creates a shader from shader code"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Inputs can be created directly

    // Outputs
    Output<globjects::Program *>              program; ///< the shader object


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
    ProgramStage(Environment * environment, const std::string & name = "ProgramStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ProgramStage();


protected:
    // Virtual Stage interface
    virtual void onProcess(AbstractGLContext * context) override;
    void onContextInit(AbstractGLContext * content) override;

protected:
    globjects::ref_ptr<globjects::Program> m_program; ///< Program object
};


} // namespace gloperate
