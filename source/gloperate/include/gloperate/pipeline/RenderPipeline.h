
#pragma once


#include <glm/glm.hpp>

#include <gloperate/pipeline/Pipeline.h>

#include <gloperate/pipeline/InputSlot.h>
#include <gloperate/pipeline/Data.h>


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
    // Inputs
    InputSlot<glm::vec4> deviceViewport;  ///< Viewport (in real device coordinates)
    InputSlot<glm::vec4> virtualViewport; ///< Viewport (in virtual coordinates)
    InputSlot<glm::vec3> backgroundColor; ///< Background color (RGB)
    InputSlot<int>       frameCounter;    ///< Frame counter (number of frames)
    InputSlot<float>     timeDelta;       ///< Time delta since last frame (in seconds)

    // Outputs
    Data<bool>           redrawNeeded;    ///< 'true' if stage needs redrawing


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
    RenderPipeline(ViewerContext * viewerContext, Pipeline * parent = nullptr);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~RenderPipeline();
};


} // namespace gloperate
