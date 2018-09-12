
#pragma once


#include <glm/glm.hpp>

#include <cppexpose/signal/Signal.h>

#include <gloperate/base/CachedValue.h>


namespace gloperate
{


/**
*  @brief
*    Represents matrices for a camera within a virtual scene.
*
*    The camera provides access to view and projection matrices that can be configured
*    as well as accessors for derived inverted matrices, their products and a
*    normal matrix.
*
*    For convenience, setters that wrap glm camera functions are provided.
*/
class GLOPERATE_API Camera
{
public:
    cppexpose::Signal<> invalidated; ///< Called when the camera has been modified


public:
    /**
    *  @brief
    *    Constructor
    */
    Camera();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Camera();

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
    *    Set view matrix
    *
    *  @param[in] matrix
    *    View matrix
    */
    void setViewMatrix(const glm::mat4 & matrix);

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
    *    Set projection matrix
    *
    *  @param[in] matrix
    *    Projection matrix
    */
    void setProjectionMatrix(const glm::mat4 & matrix);

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
    *    Set a look-at view matrix
    *
    *  @param[in] eye
    *    Camera position
    *  @param[in] center
    *    Look-at position
    *  @return
    *    Up-vector
    */
    void lookAt(const glm::vec3 & eye, const glm::vec3 & center, const glm::vec3 & up);

    /**
    *  @brief
    *    Set a perspective projection matrix
    *
    *  @param[in] fovy
    *    Vertical angle
    *  @param[in] ratio
    *    Aspect ratio
    *  @param[in] zNear
    *    Near plane
    *  @param[in] zFar
    *    Far plane
    */
    void perspective(float fovy, float ratio, float zNear, float zFar);

    /**
    *  @brief
    *    Set a perspective projection matrix
    *
    *  @param[in] fovy
    *    Vertical angle
    *  @param[in] viewport
    *    Target viewport, used to derive the aspect ratio
    *  @param[in] zNear
    *    Near plane
    *  @param[in] zFar
    *    Far plane
    */
    void perspective(float fovy, const glm::ivec2 & viewport, float zNear, float zFar);

    /**
    *  @brief
    *    Set a orthogonal projection matrix
    *
    *  @param[in] left
    *    Left border of viewbox
    *  @param[in] right
    *    Right border of viewbox
    *  @param[in] top
    *    Top border of viewbox
    *  @param[in] bottom
    *    Bottom border of viewbox
    *  @param[in] zNear
    *    Near plane
    *  @param[in] zFar
    *    Far plane
    */
    void ortho(float left, float right, float bottom, float top, float zNear, float zFar);

    /**
    *  @brief
    *    Set a orthogonal projection matrix
    *
    *  @param[in] fovy
    *    Vertical angle
    *  @param[in] ratio
    *    Aspect ratio
    *  @param[in] zNear
    *    Near plane
    *  @param[in] zFar
    *    Far plane
    */
    void ortho(float fovy, float aspect, float zNear, float zFar);

    /**
    *  @brief
    *    Recompute eye from view matrix
    *
    *  @return
    *    The position of the camera in world space
    */
    glm::vec3 eyeFromViewMatrix() const;


protected:
    /**
    *  @brief
    *    Update camera matrices
    */
    void update() const;

    /**
    *  @brief
    *    Emit signal that camera has been modified
    */
    void changed();

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
    // Camera matrices
                           glm::mat4  m_viewMatrix;                   ///< View matrix
                           glm::mat4  m_projectionMatrix;             ///< Projection matrix
    gloperate::CachedValue<glm::mat4> m_viewInvertedMatrix;           ///< Inverted view matrix
    gloperate::CachedValue<glm::mat4> m_projectionInvertedMatrix;     ///< Inverted projection matrix
    gloperate::CachedValue<glm::mat4> m_viewProjectionMatrix;         ///< View-projection matrix
    gloperate::CachedValue<glm::mat4> m_viewProjectionInvertedMatrix; ///< Invertex view-projection matrix
    gloperate::CachedValue<glm::mat3> m_normalMatrix;                 ///< Normal matrix
};


} // namespace gloperate
