/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once


#include <glm/glm.hpp>
#include <globjects/base/Referenced.h>
#include <globjects/base/CachedValue.h>
#include <gloperate/gloperate_api.h>

#include <gloperate/signals/Signal.h>


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
    Camera(const glm::vec3 & eye    = glm::vec3(0.0, 0.0, 1.0),
           const glm::vec3 & center = glm::vec3(0.0, 0.0, 0.0),
           const glm::vec3 & up     = glm::vec3(0.0, 1.0, 0.0) );

    virtual ~Camera();

    bool autoUpdating() const;
    void setAutoUpdating(bool b);

    const glm::vec3 & eye() const;
    void setEye(const glm::vec3 & eye);
    const glm::vec3 & center() const;
    void setCenter(const glm::vec3 & center);
    const glm::vec3 & up() const;
    void setUp(const glm::vec3 & up);

    float zNear() const;
    void setZNear(float zNear);
    float zFar() const;
    void setZFar(float zFar);

    float fovy() const;
    void setFovy(float fovy);

    float aspectRatio() const;
    void setAspectRatio(float ratio);
    void setAspectRatio(const glm::ivec2 & viewport);

    // lazy matrices getters

    const glm::mat4 & view() const;
    const glm::mat4 & projection() const;
    const glm::mat4 & viewProjection() const;
    const glm::mat4 & viewInverted() const;
    const glm::mat4 & projectionInverted() const;
    const glm::mat4 & viewProjectionInverted() const;

    const glm::mat3 & normal() const;

    void update() const;

    void changed();


protected:
    void dirty(bool update = true);
    void invalidateMatrices() const;


protected:
    mutable bool m_dirty;
    bool m_autoUpdate;

    glm::vec3 m_eye;
    glm::vec3 m_center;
    glm::vec3 m_up;

    float m_fovy;
    float m_aspect;
    float m_zNear;
    float m_zFar;

    globjects::CachedValue<glm::mat4> m_view;
    globjects::CachedValue<glm::mat4> m_viewInverted;
    globjects::CachedValue<glm::mat4> m_projection;
    globjects::CachedValue<glm::mat4> m_projectionInverted;
    globjects::CachedValue<glm::mat4> m_viewProjection;
    globjects::CachedValue<glm::mat4> m_viewProjectionInverted;
    globjects::CachedValue<glm::mat3> m_normal;

public:
    Signal<> invalidated;
};


} // namespace gloperate
