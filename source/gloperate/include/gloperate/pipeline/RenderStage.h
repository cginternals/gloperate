
#pragma once


#include <glm/glm.hpp>

#include <gloperate/pipeline/Stage.h>

#include <gloperate/pipeline/InputSlot.h>
#include <gloperate/pipeline/Data.h>


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
    *  @param[in] name
    *    Stage name
    *  @param[in] parent
    *    Parent pipeline (can be null)
    */
    RenderStage(ViewerContext * viewerContext, const std::string & name = "RenderStage", Pipeline * parent = nullptr);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~RenderStage();
};


} // namespace gloperate
