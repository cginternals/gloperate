
#pragma once


#include <glm/glm.hpp>

#include <cppexpose/signal/Signal.h>

#include <gloperate/base/CachedValue.h>


namespace gloperate
{


/**
*  @brief
*    Represents matrices for a typical 3D look-at camera.
*/
class GLOPERATE_API AbstractCamera
{
public:
    cppexpose::Signal<> invalidated; ///< Called when the camera has been modified


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
    AbstractCamera(const glm::vec3 & eye    = glm::vec3(0.0, 0.0, 1.0),
           const glm::vec3 & center = glm::vec3(0.0, 0.0, 0.0),
           const glm::vec3 & up     = glm::vec3(0.0, 1.0, 0.0) );

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractCamera();

    /**
    *  @brief
    *    Check if camera is updated automatically
    *
    *  @return
    *    'true' if camera is updated automatically, else 'false'
    */
    bool autoUpdating() const;

    /**
    *  @brief
    *    Set if camera is updated automatically
    *
    *  @param[in] autoUpdate
    *    'true' if camera is updated automatically, else 'false'
    */
    void setAutoUpdating(bool autoUpdate);

    /**
    *  @brief
    *    Update camera matrices
    */
    void update() const;

    /**
    *  @brief
    *    Get camera (eye) position
    *
    *  @return
    *    Camera position
    */
    const glm::vec3 & eye() const;

    /**
    *  @brief
    *    Set camera (eye) position
    *
    *  @param[in] eye
    *    Camera position
    */
    void setEye(const glm::vec3 & eye);

    /**
    *  @brief
    *    Get look-at (center) position
    *
    *  @return
    *    Look-at position
    */
    const glm::vec3 & center() const;

    /**
    *  @brief
    *    Set look-at (center) position
    *
    *  @param[in] center
    *    Look-at position
    */
    void setCenter(const glm::vec3 & center);

    /**
    *  @brief
    *    Get up-vector
    *
    *  @return
    *    Up-vector
    */
    const glm::vec3 & up() const;

    /**
    *  @brief
    *    Set up-vector
    *
    *  @param[in] up
    *    Up-vector
    */
    void setUp(const glm::vec3 & up);

    /**
    *  @brief
    *    Get near plane
    *
    *  @return
    *    Near plane
    */
    float zNear() const;

    /**
    *  @brief
    *    Set near plane
    *
    *  @param[in] zNear
    *    Near plane
    */
    void setZNear(float zNear);

    /**
    *  @brief
    *    Get far plane
    *
    *  @return
    *    Far plane
    */
    float zFar() const;

    /**
    *  @brief
    *    Set far plane
    *
    *  @param[in] zFar
    *    Far plane
    */
    void setZFar(float zFar);


public:
    // lazy matrices getters

    /**
    *  @brief
    *    Get view matrix
    *
    *  @return
    *    View matrix
    */
    const glm::mat4 & viewMatrix() const;

    /**
    *  @brief
    *    Get projection matrix
    *
    *  @return
    *    Projection matrix
    */
    const glm::mat4 & projectionMatrix() const;

    /**
    *  @brief
    *    Get view-projection matrix
    *
    *  @return
    *    View-projection matrix
    */
    const glm::mat4 & viewProjectionMatrix() const;

    /**
    *  @brief
    *    Get inverted view matrix
    *
    *  @return
    *    Inverted view matrix
    */
    const glm::mat4 & viewInvertedMatrix() const;

    /**
    *  @brief
    *    Get inverted projection matrix
    *
    *  @return
    *    Inverted projection matrix
    */
    const glm::mat4 & projectionInvertedMatrix() const;

    /**
    *  @brief
    *    Get inverted view-projection matrix
    *
    *  @return
    *    Inverted view-projection matrix
    */
    const glm::mat4 & viewProjectionInvertedMatrix() const;

    /**
    *  @brief
    *    Get normal matrix
    *
    *  @return
    *    Normal matrix
    */
    const glm::mat3 & normalMatrix() const;

    /**
    *  @brief
    *    Emit signal that camera has been modified
    */
    void changed();


protected:
    /**
    *  @brief
    *    Mark data dirty
    *
    *  @param[in] update
    *    Update matrices immediately (true), or later (false)?
    *
    *  @remarks
    *    After calling this function, the matrices will be recalculated the next time they're accessed.
    */
    void dirty(bool update = true);

    /**
    *  @brief
    *    Invalidate matrices, causing them to be recalculated
    */
    void invalidateMatrices() const;

    /**
    *  @brief
    *    Compute and return the projection matrix
    *
    *  @return
    *    The projection matrix
    */
    virtual glm::mat4 computeProjectionMatrix() const = 0;


protected:
    // Internal data
    mutable bool m_dirty; ///< Has the data been changed? If true, matrices will be recalculated
    bool m_autoUpdate;    ///< 'true' if camera is updated automatically, else 'false'

    // Camera data
    glm::vec3 m_eye;      ///< Camera position
    glm::vec3 m_center;   ///< Look-at position
    glm::vec3 m_up;       ///< Up-vector

    // Projection data
    float     m_zNear;    ///< Near plane
    float     m_zFar;     ///< Far plane

    // Camera matrices
    gloperate::CachedValue<glm::mat4> m_viewMatrix;                   ///< View matrix
    gloperate::CachedValue<glm::mat4> m_viewInvertedMatrix;           ///< Inverted view matrix
    gloperate::CachedValue<glm::mat4> m_projectionMatrix;             ///< Projection matrix
    gloperate::CachedValue<glm::mat4> m_projectionInvertedMatrix;     ///< Inverted projection matrix
    gloperate::CachedValue<glm::mat4> m_viewProjectionMatrix;         ///< View-projection matrix
    gloperate::CachedValue<glm::mat4> m_viewProjectionInvertedMatrix; ///< Invertex view-projection matrix
    gloperate::CachedValue<glm::mat3> m_normalMatrix;                 ///< Normal matrix
};


} // namespace gloperate
