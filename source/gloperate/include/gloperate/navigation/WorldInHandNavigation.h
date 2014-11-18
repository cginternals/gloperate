#pragma once

#include <gloperate/gloperate_api.h>
#include <gloperate/navigation/AbstractInteraction.h>

#include <glm/glm.hpp>

//#include "AxisAlignedBoundingBox.h"

namespace gloperate
{

class CameraCapability;
class AbstractViewportCapability;
class AbstractCoordinateProviderCapability;

class GLOPERATE_API WorldInHandNavigation : public AbstractInteraction
{

	enum InteractionMode
	{
		NoInteraction
	,	PanInteraction
	,	RotateInteraction
	};

public:
    WorldInHandNavigation(CameraCapability * cameraCapability, AbstractViewportCapability * viewportCapability,AbstractCoordinateProviderCapability * coordProviderCapability);
    virtual ~WorldInHandNavigation();

//    void setBoundaryHint(const AxisAlignedBoundingBox & aabb);
//    void setCoordinateProvider(AbstractCoordinateProvider * provider);

	virtual void reset(bool update = true);

	void panBegin(const glm::ivec2 & mouse);
  	void panProcess(const glm::ivec2 & mouse);
 	void panEnd();

//	void rotatingBegin(const glm::ivec2 & mouse);
//	void rotatingProcess(const glm::ivec2 & mouse);
//	void rotatingEnd();

    void pan(glm::vec3 t);
    void rotate(float hAngle, float vAngle);

//	void scaleAtCenter(float scale);
//	void scaleAtMouse(const glm::ivec2 & mouse,	float scale);
//	void resetScaleAtMouse(const glm::ivec2 & mouse);

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
    //Camera * m_camera;
    CameraCapability * m_cameraCapability;
    AbstractViewportCapability * m_viewportCapability;

    // TODO clean up old
//    AxisAlignedBoundingBox m_aabb;

    AbstractCoordinateProviderCapability * m_coordProviderCapability;

    bool m_rotationHappened;
	InteractionMode m_mode;

    glm::vec3  m_referencePosition;
    glm::vec3  m_modifiedPosition;
    bool  m_refPositionValid; // stores if initial interaction pick yielded valid depth
    glm::vec3   m_eye;
    glm::vec3   m_center;
    glm::vec2    m_m0;
};

} // namespace gloperate
