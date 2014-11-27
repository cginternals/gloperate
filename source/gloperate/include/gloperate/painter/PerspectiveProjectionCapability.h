/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once

#include <gloperate/gloperate_api.h>

#include <gloperate/painter/AbstractPerspectiveProjectionCapability.h>

#include <glm/glm.hpp>

#include <globjects/base/CachedValue.h>

namespace gloperate {

/**
*  @brief
*    Base class for painter camera capabilities
*
*/
class GLOPERATE_API PerspectiveProjectionCapability : public AbstractPerspectiveProjectionCapability
{


public:
    /**
    *  @brief
    *    Constructor
    */
    PerspectiveProjectionCapability(AbstractViewportCapability * viewportCapability);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~PerspectiveProjectionCapability();

    virtual float zNear() const override;
    virtual void setZNear(float zNear) override;
    virtual float zFar() const override;
    virtual void setZFar(float zFar) override;

    virtual float fovy() const override;
    virtual void setFovy(float fovy) override;

    virtual float aspectRatio() const override;

    virtual const glm::mat4 & projection() const override;
    virtual const glm::mat4 & projectionInverted() const override;

    void update() const;

    void changed();

protected:
    void dirty(bool update = true);
    void invalidateMatrices() const;

    virtual void setAspectRatio(float ratio) override;
    virtual void setAspectRatio(const glm::ivec2 & viewport) override;

protected:
    mutable bool m_dirty;

    float m_fovy;
    float m_aspect;
    float m_zNear;
    float m_zFar;

    globjects::CachedValue<glm::mat4> m_projection;
    globjects::CachedValue<glm::mat4> m_projectionInverted;

public:
    Signal<> invalidated;

};


} // namespace gloperate
