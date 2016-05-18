
#pragma once


#include <glm/glm.hpp>

#include <gloperate/base/GlmProperties.h>

#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Output.h>


namespace globjects {
    class Framebuffer;
}


namespace gloperate
{


/**
*  @brief
*    Viewer input connections
*
*    Stage that contains the inputs for a RenderSurface.
*/
class GLOPERATE_API ViewerInputs : public Stage
{
public:
    // Render stage inputs
    Output<glm::vec4>                deviceViewport;  ///< Viewport (in real device coordinates)
    Output<glm::vec4>                virtualViewport; ///< Viewport (in virtual coordinates)
    Output<glm::vec3>                backgroundColor; ///< Background color (RGB)
    Output<int>                      frameCounter;    ///< Frame counter (number of frames)
    Output<float>                    timeDelta;       ///< Time delta since last frame (in seconds)
    Output<globjects::Framebuffer *> targetFBO;       ///< Target FBO. If null, the stage is supposed to render into the default frame buffer.


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
    ViewerInputs(ViewerContext * viewerContext, Pipeline * parent = nullptr);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ViewerInputs();
};


} // namespace gloperate
