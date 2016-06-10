
#pragma once


#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/stages/interfaces/RenderInterface.h>


namespace globjects {
    class Framebuffer;
}


namespace gloperate
{


/**
*  @brief
*    Pipeline that can be used as a rendering stage for a viewer
*
*  @see
*    RenderStage
*/
class GLOPERATE_API RenderPipeline : public Pipeline
{
public:
    // Interfaces
    RenderInterface<Pipeline> renderInterface; ///< Interface for rendering into a viewer


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewerContext
    *    Viewer context to which the stage belongs (must NOT be null!)
    *  @param[in] name
    *    Stage name
    */
    RenderPipeline(ViewerContext * viewerContext, const std::string & name = "RenderPipeline");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~RenderPipeline();
};


} // namespace gloperate
