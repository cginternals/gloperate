
#pragma once


#include <glm/glm.hpp>

#include <cppexpose/plugin/plugin_api.h>

#include <glbinding/gl/gl.h>

#include <globjects/base/ref_ptr.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>


namespace globjects {
    class Program;
}

namespace cppassist {
    class FilePath;
}

namespace gloperate
{

class ProgramStage;


/**
*  @brief
*    Stage that creates a program from multiple shader paths
*
*    The paths should be of the type cppassist::FilePath.
*    The type is inferred automatically from the file ending,
*    see ShaderLoaderStage for a list.
*/
class GLOPERATE_API BasicProgramStage : public Pipeline
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        BasicProgramStage, gloperate::Stage
      , ""   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Subpipeline that creates a program from multiple shaders paths"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Inputs can be created directly, should be of type cppassist::FilePath

    // Outputs
    Output<globjects::Program *> program; ///< the program object


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
    BasicProgramStage(Environment * environment, const std::string & name = "BasicProgramStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~BasicProgramStage();


protected:
    void onInputAdded(gloperate::AbstractSlot * addedInput);
    void onInputRemoved(gloperate::AbstractSlot * removedInput);

private:
    ProgramStage * m_programStage;                ///< Stage which creates the final program

    static const std::string s_loaderStagePrefix; ///< prefix for shader loader stages
    static const std::string s_shaderStagePrefix; ///< prefix for shader stages
};


} // namespace gloperate
