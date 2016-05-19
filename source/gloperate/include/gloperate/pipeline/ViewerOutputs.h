
#pragma once


#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>


namespace gloperate
{


/**
*  @brief
*    Viewer output connections
*
*    Stage that contains the outputs for a RenderSurface.
*/
class GLOPERATE_API ViewerOutputs : public Stage
{
public:
    // Render stage outputs
    Input<bool> rendered; ///< 'true' if output has been rendered


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewerContext
    *    Viewer context to which the stage belongs (must NOT be null!)
    *  @param[in] parent
    *    Parent pipeline (can be null)
    */
    ViewerOutputs(ViewerContext * viewerContext, Pipeline * parent = nullptr);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ViewerOutputs();
};


} // namespace gloperate
