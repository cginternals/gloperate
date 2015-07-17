
#pragma once


#include <gloperate/ext-includes-begin.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <gloperate/ext-includes-end.h>


namespace gloperate
{


float pointToPlaneDistance(
    const glm::vec3 & location
,   const glm::vec3 & normal
,   const glm::vec3 & point);

const glm::mat4 rotate(
    const glm::vec3 & a
,   const glm::vec3 & b);

const glm::mat4 transform(
    const glm::vec3 & location
,   const glm::vec3 & normal);

const glm::vec3 intersection(
    bool & intersects
,   const glm::vec3 & location
,   const glm::vec3 & normal
,    const glm::vec3 & rnear
,    const glm::vec3 & rfar);


} // namespace gloperate
