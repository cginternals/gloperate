
#pragma once


#include <gloperate/ext-includes-begin.h>
#include <glm/glm.hpp>
#include <gloperate/ext-includes-end.h>

#include <gloperate/gloperate_api.h>

namespace gloperate
{

class AbstractCameraCapability;
class AbstractViewportCapability;
class AbstractProjectionCapability;
class CoordinateProvider;


/**
*  @brief
*    Camera navigation 'TreeMapNavigation'
*
*    This interaction technique is based on the 'world in hand' metaphor
*    to control a virtual camera. It allows the user to pan, rotate,
*    and zoom, based on a reference point the camera currently looks at.
*    It is specialised to interact with Treemaps and can drive a CombinedProjectionCapability
*
*  @remarks
*    This interaction technique needs a depth buffer image to work
*    correctly, which is provided by the specified CoordinateProvider.
*/
class GLOPERATE_API TreeMapNavigation
{
public:
    TreeMapNavigation(gloperate::AbstractCameraCapability & cameraCapability,
        gloperate::AbstractViewportCapability & viewportCapability,
        gloperate::CoordinateProvider & coordProvider,
        gloperate::AbstractProjectionCapability * projectionCapability);
    virtual ~TreeMapNavigation();

    virtual void reset();

    void panBegin(const glm::ivec2 & mouse);
    void panProcess(const glm::ivec2 & mouse);

    void rotateBegin(const glm::ivec2 & mouse);
    void rotateProcess(const glm::ivec2 & mouse);

    void pan(glm::vec3 t);
    void rotate(float hAngle, float vAngle);

    void scaleAtCenter(float scale);

    /**
    *  @brief
    *    zoom the camera according to position and scale
    *
    *  @param[in] mouse
    *    mouse position
    *
    *  @param[in] scale
    *    requires a value between -1 and 1
    *    scale < 0: zoom out
    *    scale > 0: zoom in
    */
    void scaleAtMouse(const glm::ivec2 & mouse,	float scale);
    void resetScaleAtMouse(const glm::ivec2 & mouse);

    // constraints
    void enforceRotationConstraints(
        float & hAngle
    ,	float & vAngle) const;

    void enforceTranslationConstraints(glm::vec3 & delta);

    void enforceRotationConstraints();
    void enforceTranslationConstraints();



    // math

    const glm::vec3 mouseRayPlaneIntersection(
        bool & intersects
    ,   const glm::ivec2 & mouse) const;
    const glm::vec3 mouseRayPlaneIntersection(
        bool & intersects
    ,   const glm::ivec2 & mouse
    ,   const glm::vec3 & planePosition
    ,   const glm::vec3 & planeNormal) const;
    const glm::vec3 clampPointToMap(glm::vec3 point) const;

protected:
    gloperate::AbstractCameraCapability & m_cameraCapability;
    gloperate::AbstractViewportCapability & m_viewportCapability;
    gloperate::AbstractProjectionCapability * m_projectionCapability;

    gloperate::CoordinateProvider & m_coordProvider;

    glm::vec3   m_referencePosition;
    bool        m_refPositionValid; // stores if initial interaction pick yielded valid depth
    glm::ivec2  m_m0;
    glm::vec3   m_cardinalDirection;
};

} // namespace gloperate
