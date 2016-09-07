#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/fwd.hpp>

#include <gloperate/gloperate_api.h>

namespace gloperate
{

class GLOPERATE_API Trackball
{
public:
	Trackball();

	/**
	 * from and to normalized to [-1, 1]
	 */
	void rotate(const glm::dvec2 & from, const glm::dvec2 & to);

	glm::dquat orientation() const;
	glm::dmat3 orientationMatrix() const;

	void reset();
	void reset(const glm::dmat3& orientation);

	void setRadius(double radius);

protected:
    glm::dvec3 mapToSphere(const glm::dvec2 & pos) const;
    double depth(const glm::dvec2 & pos) const;

protected:
	double m_radius;
	glm::dquat m_orientation;
};

} // namespace gloperate
