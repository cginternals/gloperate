
#include <gloperate/painter/PerspectiveProjectionCapability.h>

#include <gloperate/ext-includes-begin.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <gloperate/ext-includes-end.h>

#include <gloperate/painter/AbstractViewportCapability.h>


namespace gloperate
{


PerspectiveProjectionCapability::PerspectiveProjectionCapability(AbstractViewportCapability * viewportCapability)
: AbstractPerspectiveProjectionCapability(viewportCapability)
, m_dirty(false)
, m_fovy(glm::radians(40.f))
, m_aspect(1.f)
, m_zNear(0.1f)
, m_zFar(64.0f)
{
}

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
    setChanged(true);
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
    setChanged(true);
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
    setChanged(true);
}

float PerspectiveProjectionCapability::aspectRatio() const
{
    return m_aspect;
}

const glm::mat4 & PerspectiveProjectionCapability::projection() const
{
    if (m_dirty)
        update();

    if (!m_projection.isValid())
        m_projection.setValue(projectionForAspectRatio(m_aspect));

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

glm::mat4 PerspectiveProjectionCapability::projectionForAspectRatio(float ratio) const
{
    return glm::perspective(m_fovy, ratio, m_zNear, m_zFar);
}

void PerspectiveProjectionCapability::update() const
{
    if (!m_dirty)
        return;

    invalidateMatrices();

    m_dirty = false;
}

void PerspectiveProjectionCapability::dirty(bool update)
{
    m_dirty = true;

    if (update)
        this->update();
}

void PerspectiveProjectionCapability::invalidateMatrices() const
{
    m_projection.invalidate();
    m_projectionInverted.invalidate();
}

void PerspectiveProjectionCapability::setAspectRatio(float ratio)
{
    if (ratio > glm::epsilon<float>())
    {
        m_aspect = ratio;

        dirty();
        setChanged(true);
    }
}

void PerspectiveProjectionCapability::setAspectRatio(const glm::ivec2 & viewport)
{
    m_aspect = glm::max(static_cast<float>(viewport.x), 1.0f) / glm::max(static_cast<float>(viewport.y), 1.f);

    dirty();
    setChanged(true);
}


} // namespace gloperate
