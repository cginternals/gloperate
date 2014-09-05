#pragma once

#include <glm/glm.hpp>

#include <cgutils/cgutils_api.h>

namespace cgutils 
{

float pointToPlaneDistance(
    const glm::vec3 & location
,   const glm::vec3 & normal
,   const glm::vec3 & point);

const glm::mat4 CGUTILS_API rotate(
    const glm::vec3 & a
,   const glm::vec3 & b);

const glm::mat4 CGUTILS_API transform(
    const glm::vec3 & location
,   const glm::vec3 & normal);

const glm::vec3 CGUTILS_API intersection(
	bool & intersects
,   const glm::vec3 & location
,   const glm::vec3 & normal
,	const glm::vec3 & rnear
,	const glm::vec3 & rfar);

} // namespace gloutils
