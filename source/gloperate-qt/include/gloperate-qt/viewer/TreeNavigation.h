
#pragma once


#include <gloperate/ext-includes-begin.h>
#include <glm/glm.hpp>
#include <gloperate/ext-includes-end.h>

#include <gloperate-qt/gloperate-qt_api.h>
#include <gloperate/navigation/AbstractInteraction.h>


namespace gloperate
{

class AbstractCameraCapability;
class AbstractViewportCapability;
class AbstractProjectionCapability;
class CoordinateProvider;

}


/**
*  @brief
*    Camera navigation 'world in hand'
*
*    This interaction technique provides a 'world in hand' metaphor
*    to control a virtual camera. It allows the user to pan, rotate,
*    and zoom, based on a reference point the camera currently looks at.
*
*  @remarks
*    This interaction technique needs a depth buffer image to work
*    correctly, which is provided by the specified CoordinateProvider.
*/
class GLOPERATE_QT_API TreeNavigation : public gloperate::AbstractInteraction
{
public:
    /**
    *  @brief
    *    Current interaction mode
    */
    enum InteractionMode
    {
        NoInteraction
    ,	PanInteraction
    ,	RotateInteraction
    };


public:
    TreeNavigation(gloperate::AbstractCameraCapability & cameraCapability,
        gloperate::AbstractViewportCapability & viewportCapability,
        gloperate::CoordinateProvider & coordProvider, 
        gloperate::AbstractProjectionCapability * projectionCapability);
    virtual ~TreeNavigation();

    virtual void reset();

    InteractionMode mode() const;

    void panBegin(const glm::ivec2 & mouse);
    void panProcess(const glm::ivec2 & mouse);
    void panEnd();

    void rotateBegin(const glm::ivec2 & mouse);
    void rotateProcess(const glm::ivec2 & mouse);
    void rotateEnd();

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

    bool m_rotationHappened;
    InteractionMode m_mode;

    glm::vec3   m_referencePosition;
    glm::vec3   m_modifiedPosition;
    bool        m_refPositionValid; // stores if initial interaction pick yielded valid depth
    glm::vec3   m_eye;
    glm::vec3   m_center;
    glm::ivec2  m_m0;
    glm::vec3   m_cardinalDirection;
};
