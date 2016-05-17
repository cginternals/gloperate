
#pragma once


#include <glm/glm.hpp>

#include <gloperate/base/GlmProperties.h>

#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Data.h>


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
    Data<glm::vec4> deviceViewport;  ///< Viewport (in real device coordinates)
    Data<glm::vec4> virtualViewport; ///< Viewport (in virtual coordinates)
    Data<glm::vec3> backgroundColor; ///< Background color (RGB)
    Data<int>       frameCounter;    ///< Frame counter (number of frames)
    Data<float>     timeDelta;       ///< Time delta since last frame (in seconds)
    Data<globjects::Framebuffer *> fbo;


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
