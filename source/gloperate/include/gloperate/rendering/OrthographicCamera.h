
#pragma once


#include <glm/glm.hpp>

#include <gloperate/rendering/AbstractCamera.h>


namespace gloperate
{


/**
*  @brief
*    Represents matrices for a 3D orthographic look-at camera.
*/
class GLOPERATE_API OrthographicCamera : public AbstractCamera
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] eye
    *    Camera position
    *  @param[in] center
    *    Look-at position
    *  @param[in] up
    *    Up-vector
    */
    OrthographicCamera(const glm::vec3 & eye    = glm::vec3(0.0, 0.0, 1.0),
           const glm::vec3 & center = glm::vec3(0.0, 0.0, 0.0),
           const glm::vec3 & up     = glm::vec3(0.0, 1.0, 0.0) );

    /**
    *  @brief
    *    Destructor
    */
    virtual ~OrthographicCamera();

    /**
    *  @brief
    *    Get viewbox
    *
    *  @return
    *    Viewbox
    */
    const glm::vec4 & viewbox() const;

    /**
    *  @brief
    *    Set viewbox
    *
    *  @param[in] viewbox
    *    New viewbox
    */
    void setViewbox(const glm::vec4 & viewbox);

    /**
    *  @brief
    *    Set viewbox
    *
    *  @param[in] left
    *    New left coordinate
    *  @param[in] right
    *    New right coordinate
    *  @param[in] bottom
    *    New bottom coordinate
    *  @param[in] top
    *    New top coordinate
    */
    void setViewbox(float left, float right, float bottom, float top);


protected:
    /**
    *  @brief
    *    Compute and return the projection matrix
    *
    *  @return
    *    The projection matrix
    */
    virtual glm::mat4 computeProjectionMatrix() const override;


protected:
    // Projection data
    glm::vec4 m_viewbox; ///< The viewbox of the orthographic projection (left, right, bottom, top)
};


} // namespace gloperate
