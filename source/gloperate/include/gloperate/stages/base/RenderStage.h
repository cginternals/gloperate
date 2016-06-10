
#pragma once


#include <gloperate/pipeline/Stage.h>
#include <gloperate/stages/interfaces/RenderInterface.h>


namespace globjects {
    class Framebuffer;
}


namespace gloperate
{


/**
*  @brief
*    Stage that can be used as a rendering stage for a viewer
*
*    A render stage is a stage that renders into the current framebuffer
*    and can therefore be assigned to a viewer. It supports a specific
*    interface via input and output slots which is accessed by the viewer.
*
*    For convenience, this class may be used as a base class to create
*    rendering stages. However, it is not necessary to use this class:
*    any stage can be used as a rendering stage by adding the same input
*    and output slots as specified in this class.
*/
class GLOPERATE_API RenderStage : public Stage
{
public:
    // Interfaces
    RenderInterface<Stage> renderInterface; ///< Interface for rendering into a viewer


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
    RenderStage(ViewerContext * viewerContext, const std::string & name = "RenderStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~RenderStage();
};


} // namespace gloperate
