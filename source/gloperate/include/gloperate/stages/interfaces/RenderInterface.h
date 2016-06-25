
#pragma once


#include <gloperate/base/GlmProperties.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>
#include <gloperate/pipeline/OutputTypeSelector.h>


namespace globjects {
    class Framebuffer;
}


namespace gloperate
{


class Stage;


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
template <typename StageType>
class RenderInterface
{
public:
    // Type alias for output type
    template <typename T>
    using OutputType = gloperate::OutputType<T, StageType>;


public:
    // Inputs
    Input<glm::vec4>                deviceViewport;  ///< Viewport (in real device coordinates)
    Input<glm::vec4>                virtualViewport; ///< Viewport (in virtual coordinates)
    Input<glm::vec3>                backgroundColor; ///< Background color (RGB)
    Input<int>                      frameCounter;    ///< Frame counter (number of frames)
    Input<float>                    timeDelta;       ///< Time delta since last frame (in seconds)
    Input<globjects::Framebuffer *> targetFBO;       ///< Target FBO. If null, the stage is supposed to render into the default frame buffer.

    // Outputs
    OutputType<bool>                rendered;        ///< 'true' if output has been rendered


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


#include <gloperate/stages/interfaces/RenderInterface.inl>
