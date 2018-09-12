
#pragma once


#include <gloperate/rendering/AbstractCamera.h>


namespace gloperate
{


/**
*  @brief
*    Represents matrices for a typical 3D perspective look-at camera.
*
*    A camera is specified via near, far, fovy, as well as an eye, a center, and an up
*    vector. Camera itself does not use any OpenGL calls, but merely provides lazy
*    math to all common matrices required for affine transformation of a scene,
*    namely the view and projection matrices, their combination and all related
*    inverses (as well as a normal matrix).
*    The class relies on lazy computation of all matrices, causing less recomputations
*    of, e.g., matrices and inverse matrices requested on an irregular basis.
*/
class GLOPERATE_API PerspectiveCamera : public AbstractCamera
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
    PerspectiveCamera(const glm::vec3 & eye    = glm::vec3(0.0, 0.0, 1.0),
           const glm::vec3 & center = glm::vec3(0.0, 0.0, 0.0),
           const glm::vec3 & up     = glm::vec3(0.0, 1.0, 0.0) );

    /**
    *  @brief
    *    Destructor
    */
    virtual ~PerspectiveCamera();

    /**
    *  @brief
    *    Get field-of-view angle (Y)
    *
    *  @return
    *    Angle
    */
    float fovy() const;

    /**
    *  @brief
    *    Set field-of-view angle (Y)
    *
    *  @param[in] fovy
    *    Angle
    */
    void setFovy(float fovy);

    /**
    *  @brief
    *    Get aspect ratio (width / height)
    *
    *  @return
    *    Aspect ratio
    */
    float aspectRatio() const;

    /**
    *  @brief
    *    Set aspect ratio (width / height)
    *
    *  @param[in] ratio
    *    Aspect ratio
    */
    void setAspectRatio(float ratio);

    /**
    *  @brief
    *    Set aspect ratio by providing a viewport
    *
    *  @param[in] viewport
    *    Viewport size
    */
    void setAspectRatio(const glm::ivec2 & viewport);

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
    float     m_fovy;     ///< Field-of-view angle (Y)
    float     m_aspect;   ///< Aspect ratio (width / height)
};


} // namespace gloperate
