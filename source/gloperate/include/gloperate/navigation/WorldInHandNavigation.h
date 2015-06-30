
#pragma once


#include <gloperate/ext-includes-begin.h>
#include <glm/glm.hpp>
#include <gloperate/ext-includes-end.h>

#include <gloperate/navigation/AbstractInteraction.h>

//#include "AxisAlignedBoundingBox.h"


namespace gloperate
{


class AbstractCameraCapability;
class AbstractViewportCapability;
class CoordinateProvider;


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
class GLOPERATE_API WorldInHandNavigation : public AbstractInteraction
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
    WorldInHandNavigation(
        AbstractCameraCapability & cameraCapability, 
        AbstractViewportCapability & viewportCapability,
        CoordinateProvider & coordProvider);
    virtual ~WorldInHandNavigation();

//  void setBoundaryHint(const AxisAlignedBoundingBox & aabb);
//  void setCoordinateProvider(AbstractCoordinateProvider * provider);

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

//	void enforceWholeMapVisible(const float offset = 0.08);

	// constraints

//	void enforceTranslationConstraints(glm::vec3 & p) const;
	void enforceRotationConstraints(
		float & hAngle
	,	float & vAngle) const;
//	void enforceScaleConstraints(
//		float & scale
//	,	glm::vec3 & i) const;

	// math

	const glm::vec3 mouseRayPlaneIntersection(
        bool & intersects
    ,   const glm::ivec2 & mouse) const;
    const glm::vec3 mouseRayPlaneIntersection(
        bool & intersects
    ,   const glm::ivec2 & mouse
    ,   const glm::vec3 & planePosition
    ,   const glm::vec3 & planeNormal) const;


protected:
    AbstractCameraCapability & m_cameraCapability;
    AbstractViewportCapability & m_viewportCapability;

    // TODO clean up old
//  AxisAlignedBoundingBox m_aabb;

    CoordinateProvider & m_coordProvider;

    bool m_rotationHappened;
	InteractionMode m_mode;

    glm::vec3  m_referencePosition;
    glm::vec3  m_modifiedPosition;
    bool        m_refPositionValid; // stores if initial interaction pick yielded valid depth
    glm::vec3   m_eye;
    glm::vec3   m_center;
    glm::ivec2   m_m0;
};


} // namespace gloperate
