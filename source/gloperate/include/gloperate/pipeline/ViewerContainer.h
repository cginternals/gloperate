
#pragma once


#include <gloperate/pipeline/Pipeline.h>

#include <gloperate/pipeline/ViewerInputs.h>
#include <gloperate/pipeline/ViewerOutputs.h>


namespace gloperate
{


/**
*  @brief
*    Viewer pipeline container
*
*    The class is used as an interface between the RenderSurface and
*    the current rendering stage or pipeline. It is itself a pipeline
*    and as such at the root of the parent chain. It defines the
*    inputs and outputs that connect the render surface to the rendering
*    stage or pipeline it contains.
*/
class GLOPERATE_API ViewerContainer : public Pipeline
{
public:
    // Stages
    ViewerInputs  inputs;  ///< Viewer inputs
    ViewerOutputs outputs; ///< Viewer outputs


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewerContext
    *    Viewer context to which the stage belongs (must NOT be null!)
    */
    ViewerContainer(ViewerContext * viewerContext);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ViewerContainer();
};


} // namespace gloperate
