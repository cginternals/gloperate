
#include <gloperate/navigation/TreeMapNavigation.h>
#include <gloperate/painter/CombinedProjectionCapability.h>

#include <cassert>
#include <iostream>

#include <globjects/logging.h>

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

    static const float PROJECTION_TWEENING_THRESH = 0.03f * glm::pi<float>();
    static const float CONSTRAINT_ROT_MAX_V_UP    = 0.001f * glm::pi<float>();
    static const float CONSTRAINT_ROT_MAX_V_LO    = 0.48f * glm::pi<float>();
    static const float CONSTRAINT_ROT_MAX_H       = 0.2f  * glm::pi<float>();

    static const float MAP_EXTENT_X = 0.5f;
    static const float MAP_EXTENT_Z = 0.5f;
    static const float MAP_EXTENT = std::max(MAP_EXTENT_X, MAP_EXTENT_Z);

    static const float TRANSLATION_FREEDOM = 10.f;

    static const float CAM_SIZE = 0.01f;

    float clampWithGrace(float x, float min, float max, float grace)
    {
        if(x < min)
            x = min + grace;
        if(x > max)
            x = max - grace;

        return x;
    }
}

namespace gloperate{

TreeMapNavigation::TreeMapNavigation(gloperate::AbstractCameraCapability & cameraCapability,
    gloperate::AbstractViewportCapability & viewportCapability,
    gloperate::CoordinateProvider & coordProvider,
    AbstractProjectionCapability *projectionCapability)
: m_cameraCapability(cameraCapability)
, m_viewportCapability(viewportCapability)
, m_projectionCapability(projectionCapability)
, m_coordProvider(coordProvider)
, m_refPositionValid(false)
, m_cardinalDirection(glm::vec3(0.0,0.0,1.0))
{
    reset();
}

TreeMapNavigation::~TreeMapNavigation()
{
}

void TreeMapNavigation::reset()
{
    m_cameraCapability.reset();
}

const glm::vec3 TreeMapNavigation::mouseRayPlaneIntersection(
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

const glm::vec3 TreeMapNavigation::mouseRayPlaneIntersection(
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

const glm::vec3 TreeMapNavigation::clampPointToMap(glm::vec3 point) const
{
    point.x = glm::clamp(point.x, -MAP_EXTENT_X, MAP_EXTENT_X);
    point.z = glm::clamp(point.z, -MAP_EXTENT_Z, MAP_EXTENT_Z);
    return point;
}

void TreeMapNavigation::panBegin(const glm::ivec2 & mouse)
{
    bool intersects = false;
    m_referencePosition = mouseRayPlaneIntersection(intersects, mouse);

    m_refPositionValid = false;
    if (intersects)
    {
        const float depth = m_coordProvider.depthAt(mouse);
        m_refPositionValid = DepthExtractor::isValidDepth(depth);
    }
}

void TreeMapNavigation::panProcess(const glm::ivec2 & mouse)
{

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
    auto modifiedPosition = mouseRayPlaneIntersection(intersects, clamped, m_referencePosition, glm::vec3(0.f, 1.f, 0.f));

    if (intersects)
        pan(m_referencePosition - modifiedPosition);
}

void TreeMapNavigation::rotateBegin(const glm::ivec2 & mouse)
{
    bool intersects = false;
    glm::ivec2 middle(m_viewportCapability.width()/2, m_viewportCapability.height()/2);
    m_referencePosition = clampPointToMap(mouseRayPlaneIntersection(intersects, middle));

    const float depth = m_coordProvider.depthAt(middle);
    m_refPositionValid = intersects && DepthExtractor::isValidDepth(depth);

    m_m0 = mouse;
}

void TreeMapNavigation::rotateProcess(const glm::ivec2 & mouse)
{
    const glm::vec2 delta(m_m0 - mouse);
    m_m0 = mouse;
    // setup the degree of freedom for horizontal rotation within a single action
    const float wDeltaX = delta.x / static_cast<float>(m_viewportCapability.width());
    // setup the degree of freedom for vertical rotation within a single action
    const float wDeltaY = delta.y / static_cast<float>(m_viewportCapability.height());

    rotate(wDeltaX, wDeltaY);

    auto combCapability = dynamic_cast<CombinedProjectionCapability *>(m_projectionCapability);
    if(combCapability != nullptr)
    {

        const auto eye = m_cameraCapability.eye();
        const auto center = m_cameraCapability.center();
        const auto up = m_cameraCapability.up();
        const auto viewDir = glm::normalize(eye - center);
        const auto va = acosf(glm::dot(viewDir, up));

        auto tween_va = (va - CONSTRAINT_ROT_MAX_V_UP) / PROJECTION_TWEENING_THRESH;
        tween_va = glm::clamp(tween_va, 0.0f, 1.0f);

        combCapability->setOrthoFOV(eye, m_referencePosition);
        combCapability->setMix(tween_va);
    }


}

void TreeMapNavigation::pan(glm::vec3 t)
{
    enforceTranslationConstraints(t);
    m_cameraCapability.setEye(t + m_cameraCapability.eye());
    m_cameraCapability.setCenter(t + m_cameraCapability.center());
}

void TreeMapNavigation::rotate(
    float hAngle
,   float vAngle)
{
    const auto eye = m_cameraCapability.eye();
    const auto center = m_cameraCapability.center();

    const glm::vec3 ray(glm::normalize(center - eye));
    const glm::vec3 rotAxis(glm::cross(ray, m_cameraCapability.up()));

    hAngle *= ROTATION_HOR_DOF;
    vAngle *= ROTATION_VER_DOF;

    enforceRotationConstraints(hAngle, vAngle);

    glm::vec3 t = m_refPositionValid ? m_referencePosition : center;;

    glm::mat4x4 transform = glm::mat4x4();
    transform = glm::translate(transform, t);
    transform = glm::rotate(transform, hAngle, m_cameraCapability.up());
    transform = glm::rotate(transform, vAngle, rotAxis);
    transform = glm::translate(transform, -t);

    glm::vec4 newEye = transform * glm::vec4(eye, 1.0f);
    glm::vec4 newCenter = transform * glm::vec4(center, 1.0f);

    m_cameraCapability.setEye(glm::vec3(newEye));
    m_cameraCapability.setCenter(glm::vec3(newCenter));
}


void TreeMapNavigation::scaleAtMouse(
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

    const glm::vec3 newEye = eye + scale * (intersectPoint - eye);
    m_cameraCapability.setEye(newEye);

    // the center needs to be constrained to the ground plane, so calc the new
    // center based on the intersection with the scene and use this to obtain
    // the new viewray-groundplane intersection as new center.
    const glm::vec3 newCenter = center + scale * (intersectPoint - center);

    m_cameraCapability.setCenter(navigationmath::rayPlaneIntersection(intersects, newEye, newCenter));
}

void TreeMapNavigation::resetScaleAtMouse(const glm::ivec2 & mouse)
{
    const glm::vec3 eye = m_cameraCapability.eye();
    const glm::vec3 center = m_cameraCapability.center();

    // set the distance between pointed position in the scene and camera to
    // default distance
    bool intersects = false;
    glm::vec3 intersect = mouseRayPlaneIntersection(intersects, mouse);
    if (!intersects && !DepthExtractor::isValidDepth(m_coordProvider.depthAt(mouse)))
        return;

    float scale = (DEFAULT_DISTANCE / static_cast<float>((eye - intersect).length()));

    m_cameraCapability.setEye(intersect - scale * (intersect - eye));
    m_cameraCapability.setCenter(intersect - scale * (intersect - center));

    auto combCapability = dynamic_cast<CombinedProjectionCapability *>(m_projectionCapability);
    if(combCapability != nullptr)
    {
        glm::ivec2 middle(m_viewportCapability.width()/2, m_viewportCapability.height()/2);
        m_referencePosition = clampPointToMap(mouseRayPlaneIntersection(intersects, middle));
        combCapability->setOrthoFOV(eye, m_referencePosition);
    }
}

void TreeMapNavigation::scaleAtCenter(float scale)
{
    const glm::vec3 eye = m_cameraCapability.eye();
    const glm::vec3 center = m_cameraCapability.center();

    bool intersects = true;
    glm::vec3 i = navigationmath::rayPlaneIntersection(intersects, eye, center);
    if (!intersects)
        return;

    m_cameraCapability.setEye(eye + scale * (eye - i));
    m_cameraCapability.setCenter(center + scale * (center - i));

    auto combCapability = dynamic_cast<CombinedProjectionCapability *>(m_projectionCapability);
    if(combCapability != nullptr)
    {
        glm::ivec2 middle(m_viewportCapability.width()/2, m_viewportCapability.height()/2);
        m_referencePosition = clampPointToMap(mouseRayPlaneIntersection(intersects, middle));
        combCapability->setOrthoFOV(eye, m_referencePosition);
    }
}

void TreeMapNavigation::enforceRotationConstraints(
    float & hAngle
,   float & vAngle) const
{
    // retrieve the angle between camera-center to up and test how much closer
    // to up/down it can be rotated and clamp if required.
    auto eye = m_cameraCapability.eye();
    auto center = m_cameraCapability.center();
    auto up = m_cameraCapability.up();
    auto viewDir = glm::normalize(eye - center);

    /* auto horizontalDir = glm::normalize(viewDir - (up * glm::dot(viewDir, up)));

    auto ha = acosf(glm::dot(m_cardinalDirection, horizontalDir)); // TODO: make this signed

    ha = std::copysign(ha, glm::dot(glm::cross(m_cardinalDirection, horizontalDir), up));

    auto targetHAngle = ha + hAngle;
    targetHAngle = glm::clamp(targetHAngle, -CONSTRAINT_ROT_MAX_H, CONSTRAINT_ROT_MAX_H);
    hAngle = targetHAngle - ha; */


    auto va = acosf(glm::dot(viewDir, up));
    vAngle = glm::clamp(vAngle, CONSTRAINT_ROT_MAX_V_UP - va, CONSTRAINT_ROT_MAX_V_LO - va);

}

void TreeMapNavigation::enforceTranslationConstraints(glm::vec3 &delta)
{
    //make sure the camera does not veer into infinity
    auto tf = TRANSLATION_FREEDOM;
    auto eyePos = m_cameraCapability.eye();
    auto center = m_cameraCapability.center();

    auto newPos = glm::clamp(eyePos + delta, glm::vec3(-MAP_EXTENT_X*tf,0,-MAP_EXTENT_Z*tf), glm::vec3(MAP_EXTENT_X*tf,1+1*tf,MAP_EXTENT_Z*tf));
    delta = newPos-eyePos;

    //make sure at that the center is always "on" the map
    bool intersects;
    const glm::vec3 intersection(navigationmath::rayPlaneIntersection(intersects, eyePos+delta, center+delta));
    const glm::vec2 flatIntersect(intersection.x, intersection.z);

    if (navigationmath::insideSquare(flatIntersect, MAP_EXTENT))
        return;

    // const glm::vec2 i = navigationmath::raySquareIntersection(flatIntersect, MAP_EXTENT);
    // delta = glm::vec3(i.x, 0., i.y) - center;
}

void TreeMapNavigation::enforceRotationConstraints()
{
    /*auto eye = m_cameraCapability.eye();
    auto center = m_cameraCapability.center();
    auto up = m_cameraCapability.up();
    auto viewDir = glm::normalize(eye - center);

    auto horizontalDir = glm::normalize(viewDir - (up * glm::dot(viewDir, up)));

    auto ha = acosf(glm::dot(m_cardinalDirection, horizontalDir));
    hAngle = glm::clamp(hAngle, (CONSTRAINT_ROT_MAX_H - ha), -(CONSTRAINT_ROT_MAX_H - ha));
    //auto hDist = CONSTRAINT_ROT_MAX_H-ha;

    auto va = acosf(glm::dot(viewDir, up));
    vAngle = glm::clamp(vAngle, CONSTRAINT_ROT_MAX_V_UP - va, CONSTRAINT_ROT_MAX_V_LO - va); */
}

void TreeMapNavigation::enforceTranslationConstraints()
{

}

} // namespace gloperate
