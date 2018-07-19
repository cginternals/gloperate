
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>
#include <gloperate/input/AbstractEventConsumer.h>
#include <gloperate/rendering/Camera.h>


namespace gloperate
{


/**
*  @brief
*    Stage that implements a trackball camera navigation
*/
class GLOPERATE_API TrackballStage : public gloperate::Stage, gloperate::AbstractEventConsumer
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        TrackballStage, gloperate::Stage
      , "NavigationStage" // Tags
      , "" // Icon
      , "" // Annotations
      , "Stage that implements a trackball camera navigation"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v0.1.0"
    )


public:
    // Inputs
    Input<glm::vec4> viewport; ///< Viewport (in real device coordinates)
    Input<float> defaultPitch; ///< Default pitch to rotate against
    Input<float> defaultYaw; ///< Default yaw to rotate against
    Input<float> defaultZoom; ///< Default zoom

    // Outputs
    Output<gloperate::Camera *> camera; ///< Camera for the scene


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment to which the stage belongs (must NOT be null!)
    *  @param[in] name
    *    Stage name
    */
    TrackballStage(gloperate::Environment * environment, const std::string & name = "TrackballStage");

    /**
    * @brief
    *   Destructor
    */
    ~TrackballStage();


protected:
    // Virtual AbstractEventConsumer interface
    virtual void onEvent(gloperate::InputEvent * event) override;

    // Virtual Stage interface
    virtual void onProcess() override;


protected:
    std::unique_ptr<gloperate::Camera> m_camera; ///< Camera that is controlled by the stage

    glm::ivec2 m_lastMousePosition; ///< Last known mouse position
    bool       m_dragging;          ///< 'true' if a dragging operation is currently active, else 'false'

    float m_pitch; ///< Current rotation around the X axis
    float m_yaw;   ///< Current rotation around the Y axis
    float m_zoom;  ///< Current zoom factor
};


} // namespace gloperate
