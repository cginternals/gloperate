/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <gloperate/painter/PerspectiveProjectionCapability.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

#include <gloperate/painter/AbstractViewportCapability.h>

namespace gloperate {


/**
*  @brief
*    Constructor
*/
PerspectiveProjectionCapability::PerspectiveProjectionCapability(AbstractViewportCapability * viewportCapability)
: AbstractPerspectiveProjectionCapability(viewportCapability)
, m_dirty(false)
, m_fovy(glm::radians(40.f))
, m_aspect(1.f)
, m_zNear(0.1f)
, m_zFar(64.0f)
{
}

/**
*  @brief
*    Destructor
*/
PerspectiveProjectionCapability::~PerspectiveProjectionCapability()
{
}

float PerspectiveProjectionCapability::zNear() const
{
    return m_zNear;
}

void PerspectiveProjectionCapability::setZNear(const float zNear)
{
    if (std::abs(zNear - m_zNear) < std::numeric_limits<float>::epsilon())
        return;

    m_zNear = zNear;
    assert(m_zNear > 0.0);

    dirty();
}

float PerspectiveProjectionCapability::zFar() const
{
    return m_zFar;
}

void PerspectiveProjectionCapability::setZFar(const float zFar)
{
    if (std::abs(zFar - m_zFar) < std::numeric_limits<float>::epsilon())
        return;

    m_zFar = zFar;
    assert(m_zFar > m_zNear);

    dirty();
}

float PerspectiveProjectionCapability::fovy() const
{
    return m_fovy;
}

void PerspectiveProjectionCapability::setFovy(const float fovy)
{
    if (std::abs(fovy - m_fovy) < std::numeric_limits<float>::epsilon())
        return;

    m_fovy = fovy;
    assert(m_fovy > 0.0);

    dirty();
}

float PerspectiveProjectionCapability::aspectRatio() const
{
    return m_aspect;
}

void PerspectiveProjectionCapability::setAspectRatio(float ratio)
{
    if (ratio > glm::epsilon<float>())
    {
        m_aspect = ratio;

        dirty();
    }
}

void PerspectiveProjectionCapability::setAspectRatio(const glm::ivec2 & viewport)
{
    m_aspect = glm::max(static_cast<float>(viewport.x), 1.0f) / glm::max(static_cast<float>(viewport.y), 1.f);

    dirty();
}

void PerspectiveProjectionCapability::update() const
{
    if (!m_dirty)
        return;

    invalidateMatrices();

    m_dirty = false;

    const_cast<PerspectiveProjectionCapability*>(this)->changed();
}

void PerspectiveProjectionCapability::invalidateMatrices() const
{
    m_projection.invalidate();
    m_projectionInverted.invalidate();
}

void PerspectiveProjectionCapability::dirty(bool update)
{
    m_dirty = true;

    if (update)
        this->update();
}

void PerspectiveProjectionCapability::changed()
{
    invalidated();
}

const glm::mat4 & PerspectiveProjectionCapability::projection() const
{
    if (m_dirty)
        update();

    if (!m_projection.isValid())
        m_projection.setValue(glm::perspective(m_fovy, m_aspect, m_zNear, m_zFar));

    return m_projection.value();
}

const glm::mat4 & PerspectiveProjectionCapability::projectionInverted() const
{
    if (m_dirty)
        update();

    if (!m_projectionInverted.isValid())
        m_projectionInverted.setValue(glm::inverse(projection()));

    return m_projectionInverted.value();
}

} // namespace gloperate
