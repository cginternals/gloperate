
#pragma once


#include <glm/glm.hpp>

#include <gloperate/pipeline/Pipeline.h>

#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/ProxyOutput.h>


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
    Input<glm::vec4> deviceViewport;  ///< Viewport (in real device coordinates)
    Input<glm::vec4> virtualViewport; ///< Viewport (in virtual coordinates)
    Input<glm::vec3> backgroundColor; ///< Background color (RGB)
    Input<int>       frameCounter;    ///< Frame counter (number of frames)
    Input<float>     timeDelta;       ///< Time delta since last frame (in seconds)

    // Outputs
    ProxyOutput<bool> rendered;       ///< 'true' if output has been rendered


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewerContext
    *    Viewer context to which the stage belongs (must NOT be null!)
    *  @param[in] name
    *    Stage name
    *  @param[in] parent
    *    Parent pipeline (can be null)
    */
    RenderPipeline(ViewerContext * viewerContext, const std::string & name = "RenderPipeline", Pipeline * parent = nullptr);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~RenderPipeline();
};


} // namespace gloperate
