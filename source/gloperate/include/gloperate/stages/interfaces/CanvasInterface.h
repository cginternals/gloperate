
#pragma once


#include <gloperate/base/ExtendedProperties.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class Stage;
class RenderTarget;


/**
*  @brief
*    Interface that can be used for rendering stages and pipelines that
*    are intended to get rendered by a Canvas.
*
*    A render stage is a stage that renders into framebuffer targets
*    and can therefore be assigned to a viewer. Therefore, it has to support
*    interface via input and output slots which is accessed by the viewer.
*
*    A Canvas should attach to the render targets
*
*    This class can be used to instanciate the necessary inputs and outputs
*    for rendering stages. It can just be instanciated on a stage or pipeline
*    and it will add the inputs and outputs directly to the stage (the interface
*    itself is not an object in the hierarchy).
*/
class GLOPERATE_API CanvasInterface
{
public:
    // Inputs
    Input<glm::vec4> viewport;        ///< Viewport (in real device coordinates)
    Input<Color>     backgroundColor; ///< Background color (RGBA)
    Input<int>       frameCounter;    ///< Frame counter (number of frames)
    Input<float>     timeDelta;       ///< Time delta since last frame (in seconds)

public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] stage
    *    Stage (must NOT be null)
    */
    CanvasInterface(Stage * stage);

    /**
    *  @brief
    *    Destructor
    */
    ~CanvasInterface();
};


} // namespace gloperate
