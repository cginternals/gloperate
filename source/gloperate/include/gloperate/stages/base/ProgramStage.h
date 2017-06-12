
#pragma once


#include <cppexpose/plugin/plugin_api.h>
#include <cppexpose/signal/ScopedConnection.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>


namespace globjects
{
    class Program;
    class Shader;
}

namespace cppassist
{
    class FilePath;
}


namespace gloperate
{


/**
*  @brief
*    Stage that creates a program from multiple shaders
*
*    It expects input of pointers to globjects::Shader or cppassist::FilePath objects.
*/
class GLOPERATE_API ProgramStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        ProgramStage, gloperate::Stage
      , ""   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Stage that creates a program from multiple shaders"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Inputs of type Shader or FilePath can be created dynamically

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
    ProgramStage(Environment * environment, const std::string & name = "ProgramStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ProgramStage();


protected:
    // Virtual Stage interface
    virtual void onProcess() override;
    virtual void onContextInit(AbstractGLContext * content) override;
    virtual void onContextDeinit(AbstractGLContext * content) override;


protected:
    // OpenGL objects
    std::unique_ptr<globjects::Program>             m_program; ///< Program object
    std::vector<std::unique_ptr<globjects::Shader>> m_shaders; ///< collection of self created shaders for later removal

    // Signal connections
    cppexpose::ScopedConnection m_inputAddedConnection;
    cppexpose::ScopedConnection m_inputRemovedConnection;
};


} // namespace gloperate
