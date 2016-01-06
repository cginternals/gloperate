
#pragma once


#include <gloperate/ext-includes-begin.h>
#include <glm/glm.hpp>
#include <gloperate/ext-includes-end.h>

#include <signalzeug/Signal.h>

#include <globjects/base/Referenced.h>

#include <gloperate/gloperate_api.h>

#include <gloperate/base/CachedValue.h>

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
class GLOPERATE_API Camera : public globjects::Referenced
{
public:
    signalzeug::Signal<> invalidated;   /**< Called when the camera has been modified */


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
    Camera(const glm::vec3 & eye    = glm::vec3(0.0, 0.0, 1.0),
           const glm::vec3 & center = glm::vec3(0.0, 0.0, 0.0),
           const glm::vec3 & up     = glm::vec3(0.0, 1.0, 0.0) );

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Camera();

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

    // lazy matrices getters

    /**
    *  @brief
    *    Get view matrix
    *
    *  @return
    *    View matrix
    */
    const glm::mat4 & view() const;

    /**
    *  @brief
    *    Get projection matrix
    *
    *  @return
    *    Projection matrix
    */
    const glm::mat4 & projection() const;

    /**
    *  @brief
    *    Get view-projection matrix
    *
    *  @return
    *    View-projection matrix
    */
    const glm::mat4 & viewProjection() const;

    /**
    *  @brief
    *    Get inverted view matrix
    *
    *  @return
    *    Inverted view matrix
    */
    const glm::mat4 & viewInverted() const;

    /**
    *  @brief
    *    Get inverted projection matrix
    *
    *  @return
    *    Inverted projection matrix
    */
    const glm::mat4 & projectionInverted() const;

    /**
    *  @brief
    *    Get inverted view-projection matrix
    *
    *  @return
    *    Inverted view-projection matrix
    */
    const glm::mat4 & viewProjectionInverted() const;

    /**
    *  @brief
    *    Get normal matrix
    *
    *  @return
    *    Normal matrix
    */
    const glm::mat3 & normal() const;

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


protected:
    // Internal data
    mutable bool m_dirty;   /**< Has the data been changed? If true, matrices will be recalculated */
    bool m_autoUpdate;      /**< 'true' if camera is updated automatically, else 'false' */

    // Camera data
    glm::vec3 m_eye;        /**< Camera position */
    glm::vec3 m_center;     /**< Look-at position */
    glm::vec3 m_up;         /**< Up-vector */
    float     m_fovy;       /**< Field-of-view angle (Y) */
    float     m_aspect;     /**< Aspect ratio (width / height) */
    float     m_zNear;      /**< Near plane */
    float     m_zFar;       /**< Far plane */

    // Camera matrices
    gloperate::CachedValue<glm::mat4> m_view;                   /**< View matrix */
    gloperate::CachedValue<glm::mat4> m_viewInverted;           /**< Inverted view matrix */
    gloperate::CachedValue<glm::mat4> m_projection;             /**< Projection matrix */
    gloperate::CachedValue<glm::mat4> m_projectionInverted;     /**< Inverted projection matrix */
    gloperate::CachedValue<glm::mat4> m_viewProjection;         /**< View-projection matrix */
    gloperate::CachedValue<glm::mat4> m_viewProjectionInverted; /**< Invertex view-projection matrix */
    gloperate::CachedValue<glm::mat3> m_normal;                 /**< Normal matrix */
};


} // namespace gloperate
