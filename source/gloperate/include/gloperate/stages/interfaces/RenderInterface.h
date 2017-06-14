
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
*    Interface that can be used for rendering stages and pipelines
*
*    A render stage is a stage that renders into the current framebuffer
*    and can therefore be assigned to a viewer. Therefore, it has to support
*    interface via input and output slots which is accessed by the viewer.
*
*    This class can be used to instanciate the necessary inputs and outputs
*    for rendering stages. It can just be instanciated on a stage or pipeline
*    and it will add the inputs and outputs directly to the stage (the interface
*    itself is not an object in the hierarchy).
*/
class GLOPERATE_API RenderInterface
{
public:
    // Inputs
    Input<glm::vec4>                 deviceViewport;    ///< Viewport (in real device coordinates)
    Input<glm::vec4>                 virtualViewport;   ///< Viewport (in virtual coordinates)
    Input<Color>                     backgroundColor;   ///< Background color (RGB)
    Input<int>                       frameCounter;      ///< Frame counter (number of frames)
    Input<float>                     timeDelta;         ///< Time delta since last frame (in seconds)
    Input<gloperate::RenderTarget *> colorRenderTarget; ///< Target color attachment (must not be null)
    Input<gloperate::RenderTarget *> depthRenderTarget; ///< Target color attachment (may be null, depends on created OpenGL context)

    // Outputs
    Output<gloperate::RenderTarget *> colorRenderTargetOut; ///< Output color target of pipeline (must be set by owning stage/pipeline)
    Output<gloperate::RenderTarget *> depthRenderTargetOut; ///< Output depth target of pipeline (may be set by owning stage/pipeline)


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] stage
    *    Stage (must NOT be null)
    */
    RenderInterface(Stage * stage);

    /**
    *  @brief
    *    Destructor
    */
    ~RenderInterface();
};


} // namespace gloperate
