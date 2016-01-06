
#include <gloperate/navigation/WorldInHandNavigation.h>

#include <cassert>
#include <iostream>

#include <gloperate/ext-includes-begin.h>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <gloperate/ext-includes-end.h>

#include <gloperate/painter/AbstractCameraCapability.h>
#include <gloperate/painter/AbstractViewportCapability.h>
#include <gloperate/tools/DepthExtractor.h>
#include <gloperate/navigation/CoordinateProvider.h>
#include <gloperate/navigation/navigationmath.h>


namespace
{
    static const float DEFAULT_SCALE_STEP = 0.1f;
    static const float DEFAULT_DISTANCE   = 2.0f;
    static const float DEFAULT_DIST_MIN   = 0.1f;
    static const float DEFAULT_DIST_MAX   = 4.0f;

    static const float ROTATION_HOR_DOF   = 0.8f * glm::pi<float>();
    static const float ROTATION_VER_DOF   = 0.8f * glm::pi<float>();

    static const float ROTATION_KEY_SCALE = 1.0f;

    //static const float NAV_CONSTRAINT_PAN_CIRCLE_R = 2.83;
    static const float CONSTRAINT_ROT_MAX_V_UP = 0.02f * glm::pi<float>();
    static const float CONSTRAINT_ROT_MAX_V_LO = 0.98f * glm::pi<float>();
}


namespace gloperate
{


WorldInHandNavigation::WorldInHandNavigation(
    AbstractCameraCapability & cameraCapability, 
    AbstractViewportCapability & viewportCapability,
    CoordinateProvider & coordProvider)
: m_cameraCapability(cameraCapability)
, m_viewportCapability(viewportCapability)
, m_coordProvider(coordProvider)
, m_rotationHappened(false)
, m_mode(NoInteraction)
, m_refPositionValid(false)
{
    reset();
}

WorldInHandNavigation::~WorldInHandNavigation()
{
}

//void WorldInHandNavigation::setBoundaryHint(const AxisAlignedBoundingBox & aabb)
//{
//    m_aabb = aabb;
//}

//void WorldInHandNavigation::setCoordinateProvider(AbstractCoordinateProvider * provider)
//{
//    m_coordsProvider = provider;
//}

WorldInHandNavigation::InteractionMode WorldInHandNavigation::mode() const
{
    return m_mode;
}

void WorldInHandNavigation::reset()
{
    m_cameraCapability.reset();

    m_mode = NoInteraction;

//    enforceWholeMapVisible();
}


const glm::vec3 WorldInHandNavigation::mouseRayPlaneIntersection(
    bool & intersects,
    const glm::ivec2 & mouse, 
    const glm::vec3 & planePosition, 
    const glm::vec3 & planeNormal) const
{
    // build a ray in object space from screen space mouse position and get
    // intersection with near and far planes.

    const glm::vec3 pointNear = m_coordProvider.unproject(mouse, 0.0);
    const glm::vec3 pointFar = m_coordProvider.unproject(mouse, 1.0);

    return navigationmath::rayPlaneIntersection(intersects, pointNear, pointFar, planePosition, planeNormal);
}

const glm::vec3 WorldInHandNavigation::mouseRayPlaneIntersection(
    bool & intersects
    , const glm::ivec2 & mouse) const
{
    const float depth = m_coordProvider.depthAt(mouse);

    // no scene object was picked - simulate picking on xz-plane
    if (depth >= 1.0 - std::numeric_limits<float>::epsilon())
        // use current center to construct reference plane
        return mouseRayPlaneIntersection(intersects, mouse, m_cameraCapability.center(), glm::vec3(0.f, 1.f, 0.f));

    intersects = true;
    return m_coordProvider.unproject(mouse, depth);
}


void WorldInHandNavigation::panBegin(const glm::ivec2 & mouse)
{
    if (NoInteraction != m_mode)
        return;

    m_mode = PanInteraction;

    bool intersects = false;
    m_referencePosition = mouseRayPlaneIntersection(intersects, mouse);

    m_refPositionValid = false;
    if (intersects)
    {
        const float depth = m_coordProvider.depthAt(mouse);
        m_refPositionValid = DepthExtractor::isValidDepth(depth);
    }

    m_eye = m_cameraCapability.eye();
    m_center = m_cameraCapability.center();
}

void WorldInHandNavigation::panProcess(const glm::ivec2 & mouse)
{
    if (PanInteraction != m_mode || !m_refPositionValid)
        return;

    // The first click of the interaction yields an object space position m_referencePosition.
    // this point is our constraint for panning, that means for every mouse 
    // position there has to be an appropriate positioning for the scene, so
    // that the point under the mouse remains m_referencePosition.
    // With this point and the up normal we build a plane, that defines the 
    // panning space. For panning, a ray is created, pointing from the screen
    // pixel into the view frustum. This ray then is converted to object space
    // and used to intersect with the plane at p. 
    // The delta of m_referencePosition and p is the translation required for panning.

    // constrain mouse interaction to viewport (if disabled, could lead to mishaps)
    const glm::ivec2 clamped(
        glm::clamp(mouse.x, 0, m_viewportCapability.width()),
        glm::clamp(mouse.y, 0, m_viewportCapability.height()));

    bool intersects = false;
    m_modifiedPosition = mouseRayPlaneIntersection(intersects, clamped, m_referencePosition, glm::vec3(0.f, 1.f, 0.f));

    if (intersects)
        pan(m_referencePosition - m_modifiedPosition);
}

void WorldInHandNavigation::panEnd()
{
    if (PanInteraction != m_mode)
        return;

    m_mode = NoInteraction;
}

void WorldInHandNavigation::rotateBegin(const glm::ivec2 & mouse)
{
    if (NoInteraction != m_mode)
        return;

    m_mode = RotateInteraction;

    bool intersects = false;
    m_referencePosition = mouseRayPlaneIntersection(intersects, mouse);

    const float depth = m_coordProvider.depthAt(mouse);
    m_refPositionValid = intersects && DepthExtractor::isValidDepth(depth);

    m_m0 = mouse;

    m_eye = m_cameraCapability.eye();
    m_center = m_cameraCapability.center();
}

void WorldInHandNavigation::rotateEnd()
{
    if (RotateInteraction != m_mode)
        return;

    m_mode = NoInteraction;
}

void WorldInHandNavigation::rotateProcess(const glm::ivec2 & mouse)
{
    if (RotateInteraction != m_mode)
        return;

    const glm::vec2 delta(m_m0 - mouse);
    // setup the degree of freedom for horizontal rotation within a single action
    const float wDeltaX = delta.x / static_cast<float>(m_viewportCapability.width());
    // setup the degree of freedom for vertical rotation within a single action
    const float wDeltaY = delta.y / static_cast<float>(m_viewportCapability.height());

    rotate(wDeltaX, wDeltaY);
}

void WorldInHandNavigation::pan(glm::vec3 t)
{
    //enforceTranslationConstraints(t);

    m_cameraCapability.setEye(t + m_cameraCapability.eye());
    m_cameraCapability.setCenter(t + m_cameraCapability.center());
}

void WorldInHandNavigation::rotate(
    float hAngle
,   float vAngle)
{
    m_rotationHappened = true;

    const glm::vec3 ray(glm::normalize(m_cameraCapability.center() - m_eye));
    const glm::vec3 rotAxis(glm::cross(ray, m_cameraCapability.up()));

    hAngle *= ROTATION_HOR_DOF;
    vAngle *= ROTATION_VER_DOF;

    enforceRotationConstraints(hAngle, vAngle);

    glm::vec3 t = m_refPositionValid ? m_referencePosition : m_center;;

    glm::mat4x4 transform = glm::mat4x4();
    transform = glm::translate(transform, t);
    transform = glm::rotate(transform, hAngle, m_cameraCapability.up());
    transform = glm::rotate(transform, vAngle, rotAxis);
    transform = glm::translate(transform, -t);

    glm::vec4 newEye = transform * glm::vec4(m_eye, 0.0f);
    glm::vec4 newCenter = transform * glm::vec4(m_center, 0.0f);

    m_cameraCapability.setEye(glm::vec3(newEye));
    m_cameraCapability.setCenter(glm::vec3(newCenter));
}


void WorldInHandNavigation::scaleAtMouse(
    const glm::ivec2 & mouse
    , float scaleDelta)
{
    const glm::vec3 eye = m_cameraCapability.eye();
    const glm::vec3 center = m_cameraCapability.center();

    bool intersects = false;

    glm::vec3 intersectPoint = mouseRayPlaneIntersection(intersects, mouse);

    if (!intersects && !DepthExtractor::isValidDepth(m_coordProvider.depthAt(mouse)))
        return;

    // scale the distance between the pointed position in the scene and the 
    // camera position - using ray constraints, the center is scaled appropriately.

    float scale = glm::clamp(scaleDelta, - 1.f, 1.f);

    if (scale > 0.f)
        scale = 1.f / (1.f - scale) - 1.f; // ensure that scaling consistent in both direction
    
    // enforceScaleConstraints(scale, i);

    const glm::vec3 newEye = eye + scale * (intersectPoint - eye);
    m_cameraCapability.setEye(newEye);

    // the center needs to be constrained to the ground plane, so calc the new
    // center based on the intersection with the scene and use this to obtain 
    // the new viewray-groundplane intersection as new center.
    const glm::vec3 newCenter = center + scale * (intersectPoint - center);

    m_cameraCapability.setCenter(navigationmath::rayPlaneIntersection(intersects, newEye, newCenter));
}

void WorldInHandNavigation::resetScaleAtMouse(const glm::ivec2 & mouse)
{
    const glm::vec3 ln = m_cameraCapability.eye();
    const glm::vec3 lf = m_cameraCapability.center();

    // set the distance between pointed position in the scene and camera to 
    // default distance
    bool intersects = false;
    glm::vec3 i = mouseRayPlaneIntersection(intersects, mouse);
    if (!intersects && !DepthExtractor::isValidDepth(m_coordProvider.depthAt(mouse)))
        return;

    float scale = (DEFAULT_DISTANCE / static_cast<float>((ln - i).length()));

    //enforceScaleConstraints(scale, i);

    m_cameraCapability.setEye(i - scale * (i - ln));
    m_cameraCapability.setCenter(i - scale * (i - lf));
}

void WorldInHandNavigation::scaleAtCenter(float scale)
{
    const glm::vec3 ln = m_cameraCapability.eye();
    const glm::vec3 lf = m_cameraCapability.center();

    bool intersects = true;
    glm::vec3 i = navigationmath::rayPlaneIntersection(intersects, ln, lf);
    if (!intersects)
        return;

    //enforceScaleConstraints(scale, i);

    m_cameraCapability.setEye(ln + scale * (ln - i));
    m_cameraCapability.setCenter(lf + scale * (lf - i));
}


//void WorldInHandNavigation::enforceTranslationConstraints(glm::vec3 & p) const
//{
//    QMatrix4x4 m;
//    m.translate(p);

//    const QVector2D center(NavigationMath::xz(m * m_center));
//    if (NavigationMath::insideSquare(center))
//        return;

//    const QVector2D i = NavigationMath::raySquareIntersection(center);
//    p = glm::vec3(i.x(), 0., i.y()) - m_center;
//}

void WorldInHandNavigation::enforceRotationConstraints(
    float & /*hAngle*/
,   float & vAngle) const
{
    // hAngle is not constrained, vAngle is.

    // retrieve the angle between camera-center to up and test how much closer
    // to up/down it can be rotated and clamp if required.

    const float va = glm::degrees(acosf(
        glm::dot(glm::normalize(m_eye - m_center), m_cameraCapability.up())));

    if (vAngle <= 0.0)
		vAngle = glm::max(vAngle, glm::degrees(CONSTRAINT_ROT_MAX_V_UP) - va);
    else
		vAngle = glm::min(vAngle, glm::degrees(CONSTRAINT_ROT_MAX_V_LO) - va);
}
 
//void WorldInHandNavigation::enforceScaleConstraints(
//    float & scale
//,   glm::vec3 & i) const
//{
//    // first constraint: i must be within the ground quad...
//    QVector2D i2 = NavigationMath::xz(i);

//    if (!NavigationMath::insideSquare(i2))
//    {
//        i2 = NavigationMath::raySquareIntersection(i2);
//        i = glm::vec3(i2.x(), 0., i2.y());
//    }

//    // second constraint: scale factor must be within min and max...
//    const glm::vec3 eye = m_eye + scale * (m_eye - i);

//    const float ds = (eye - i).length();

//    if ((scale > 0. && ds >= DEFAULT_DIST_MAX)
//    ||  (scale < 0. && ds <= DEFAULT_DIST_MIN)
//    ||  (eye.y() <= m_center.y()))	// last fixes abnormal scales (e.g., resulting from mouse flywheels)
//        scale = 0.0;
//}

//void WorldInHandNavigation::enforceWholeMapVisible(const float offset)
//{
//    const float h(_swmBounds.urb.y());
//
//    const glm::vec3 bbox[8] =
//    {
//        // front
//        glm::vec3(-1.f, 0.f, +1.f)
//        , glm::vec3(+1.f, 0.f, +1.f)
//        , glm::vec3(+1.f, h, +1.f)
//        , glm::vec3(-1.f, h, +1.f)
//        // back
//        , glm::vec3(-1.f, 0.f, -1.f)
//        , glm::vec3(+1.f, 0.f, -1.f)
//        , glm::vec3(+1.f, h, -1.f)
//        , glm::vec3(-1.f, h, -1.f)
//    };
//
//    float nearest = FLT_MAX;
//
//    glm::vec3 farthestCamera = _center;
//    float farthestDistanceSquared = 0.0;
//
//    // temporaries for modelview matrix update
//    QMatrix4x4 modelView, projection, modelViewProjection;
//    float zNear, zFar;
//
//    // retrieve the closest point to the ray in view direction
//    for (int i = 0; i < 8; ++i)
//    {
//        const glm::vec3 &p(bbox[i]);
//
//        // check if the point is already visible, if not adjust camera
//        updateMatrices(farthestCamera, _center, _up, h,
//            fieldOfView(), _aspect, zNear, zFar,
//            modelView, projection, modelViewProjection);
//
//        if (NavigationUtils::pointIsVisible(modelViewProjection, p))
//            continue;
//
//        // so request new camera pos
//        const glm::vec3 newCamera = NavigationUtils::cameraWithPointInView(
//            _camera, _center, _up, fieldOfView(), _aspect, p);
//
//        const float ls = (_center - newCamera).lengthSquared();
//
//        // retrieve distance from current center
//        if (ls > farthestDistanceSquared)
//        {
//            farthestDistanceSquared = ls;
//            farthestCamera = newCamera;
//        }
//    }
//
//    // adjust distance, so that znear is always max distance to new camera
//    float d = sqrt(farthestDistanceSquared) + offset;
//
//    d = qMin<float>(d, NAV_DEFAULT_DIST_MAX);
//
//    _camera = _center + (_camera - _center).normalized() * d;
//
//    setDirty();
//}


} // namespace gloperate
