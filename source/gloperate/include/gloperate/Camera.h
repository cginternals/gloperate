#pragma once


#include <glm/glm.hpp>
#include <globjects/base/Referenced.h>
#include <globjects/base/CachedValue.h>
#include <gloperate/gloperate_api.h>


namespace gloperate
{


class GLOPERATE_API Camera : public globjects::Referenced
{


public:
    Camera(const glm::vec3 & eye    = glm::vec3(0.0, 0.0, 1.0),
           const glm::vec3 & center = glm::vec3(0.0, 0.0, 0.0),
           const glm::vec3 & up     = glm::vec3(0.0, 1.0, 0.0) );

    virtual ~Camera();

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

    const glm::ivec2 & viewport() const;
    void setViewport(const glm::ivec2 & viewport);
    void setViewport(int width, int height);

    float aspectRatio() const;

    // lazy matrices getters

    const glm::mat4 & view() const;
    const glm::mat4 & projection() const;
    const glm::mat4 & viewProjection() const;
    const glm::mat4 & viewInverted() const;
    const glm::mat4 & projectionInverted() const;
    const glm::mat4 & viewProjectionInverted() const;

    const glm::mat3 & normal() const;

    void update() const;

    virtual void changed() const;


protected:
    void dirty(bool update = true);
    void invalidateMatrices() const;


protected:
    mutable bool m_dirty;

    glm::vec3 m_eye;
    glm::vec3 m_center;
    glm::vec3 m_up;

    float m_fovy;
    float m_aspect;
    float m_zNear;
    float m_zFar;

    glm::ivec2 m_viewport;

    globjects::CachedValue<glm::mat4> m_view;
    globjects::CachedValue<glm::mat4> m_viewInverted;
    globjects::CachedValue<glm::mat4> m_projection;
    globjects::CachedValue<glm::mat4> m_projectionInverted;
    globjects::CachedValue<glm::mat4> m_viewProjection;
    globjects::CachedValue<glm::mat4> m_viewProjectionInverted;
    globjects::CachedValue<glm::mat3> m_normal;


};


} // namespace gloperate
