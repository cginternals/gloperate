#include <gloperate/navigation/Trackball.h>

#include <glm/gtx/norm.hpp>

namespace gloperate
{

Trackball::Trackball()
: m_radius(1.0)
{
}

void Trackball::reset()
{
	m_orientation = glm::dquat();
}

void Trackball::reset(const glm::dmat4& rotation)
{
    m_orientation = glm::dquat(rotation);
}

void Trackball::setRadius(double radius)
{
	m_radius = radius;
}

glm::dquat Trackball::orientation() const
{
	return m_orientation;
}

glm::dmat4 Trackball::orientationMatrix() const
{
	return glm::mat4_cast(m_orientation);
}

void Trackball::rotate(const glm::dvec2 & from, const glm::dvec2 & to)
{
	m_orientation = m_orientation * makeRotation(mapToSphere(from), mapToSphere(to));
}

glm::dvec3 Trackball::mapToSphere(const glm::dvec2 & pos) const
{
	return glm::normalize(glm::dvec3(pos, depth(pos)));
}

double Trackball::depth(const glm::dvec2 & pos) const
{
	// http://www.opengl.org/wiki/TrackballNavigation
	double d = glm::length2(pos);
	double r2 = m_radius * m_radius;

	return d <= r2 * 0.5 ? glm::sqrt(r2 - d) : r2 * 0.5 / glm::sqrt(d);
}

glm::dquat Trackball::makeRotation(const glm::dvec3 & from, const glm::dvec3& to)
{
	return glm::dquat(glm::dot(from, to), glm::cross(from, to) * m_orientation);
}

} // namespace gloperate
