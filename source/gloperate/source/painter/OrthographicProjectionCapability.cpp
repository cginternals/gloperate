
#include <gloperate/painter/OrthographicProjectionCapability.h>

#include <gloperate/ext-includes-begin.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <gloperate/ext-includes-end.h>

#include <gloperate/painter/AbstractViewportCapability.h>


namespace gloperate
{


OrthographicProjectionCapability::OrthographicProjectionCapability(AbstractViewportCapability * viewportCapability)
: AbstractOrthographicProjectionCapability(viewportCapability)
, m_dirty{false}
, m_height{1.0f}
, m_aspect{1.0f}
, m_zNear{0.1f}
, m_zFar{64.0f}
{
}

OrthographicProjectionCapability::~OrthographicProjectionCapability()
{
}

float OrthographicProjectionCapability::zNear() const
{
    return m_zNear;
}

void OrthographicProjectionCapability::setZNear(const float zNear)
{
    if (std::abs(zNear - m_zNear) < std::numeric_limits<float>::epsilon())
        return;

    m_zNear = zNear;
    assert(m_zNear > 0.0);

    dirty();
    setChanged(true);
}

float OrthographicProjectionCapability::zFar() const
{
    return m_zFar;
}

void OrthographicProjectionCapability::setZFar(const float zFar)
{
    if (std::abs(zFar - m_zFar) < std::numeric_limits<float>::epsilon())
        return;

    m_zFar = zFar;
    assert(m_zFar > m_zNear);

    dirty();
    setChanged(true);
}

float OrthographicProjectionCapability::height() const
{
    return m_height;
}

void OrthographicProjectionCapability::setHeight(const float height)
{
    if (std::abs(height - m_height) < std::numeric_limits<float>::epsilon())
        return;

    m_height = height;
    assert(m_height > 0.0);

    dirty();
    setChanged(true);
}

float OrthographicProjectionCapability::aspectRatio() const
{
    return m_aspect;
}

const glm::mat4 & OrthographicProjectionCapability::projection() const
{
    if (m_dirty)
        update();

    if (!m_projection.isValid())
        m_projection.setValue(projectionForAspectRatio(m_aspect));

    return m_projection.value();
}

const glm::mat4 & OrthographicProjectionCapability::projectionInverted() const
{
    if (m_dirty)
        update();

    if (!m_projectionInverted.isValid())
        m_projectionInverted.setValue(glm::inverse(projection()));

    return m_projectionInverted.value();
}

glm::mat4 OrthographicProjectionCapability::projectionForAspectRatio(float ratio) const
{
    const auto width = ratio * m_height;
    return glm::ortho(-width * 0.5f, width * 0.5f, -m_height * 0.5f, m_height * 0.5f, m_zNear, m_zFar);
}

void OrthographicProjectionCapability::update() const
{
    if (!m_dirty)
        return;

    invalidateMatrices();

    m_dirty = false;
}

void OrthographicProjectionCapability::dirty(bool updateNow)
{
    m_dirty = true;

    if (updateNow)
        this->update();
}

void OrthographicProjectionCapability::invalidateMatrices() const
{
    m_projection.invalidate();
    m_projectionInverted.invalidate();
}

void OrthographicProjectionCapability::setAspectRatio(float ratio)
{
    if (ratio <= glm::epsilon<float>())
    {
        return;
    }

    m_aspect = ratio;

    dirty();
    setChanged(true);
}

void OrthographicProjectionCapability::setAspectRatio(const glm::ivec2 & viewport)
{
    m_aspect = glm::max(static_cast<float>(viewport.x), 1.0f) / glm::max(static_cast<float>(viewport.y), 1.f);

    dirty();
    setChanged(true);
}


} // namespace gloperate
