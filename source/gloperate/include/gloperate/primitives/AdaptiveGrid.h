
#pragma once


#include <gloperate/ext-includes-begin.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <gloperate/ext-includes-end.h>

#include <globjects/base/ref_ptr.h>
#include <globjects/base/Referenced.h>

#include <globjects/Program.h>
#include <globjects/VertexArray.h>
#include <globjects/Buffer.h>

#include <gloperate/gloperate_api.h>
 

namespace gloperate
{


class Camera;


/**
*  @brief
*    A plane aligned regular raster with camera position based refinement
*
*    This grid alignes itself along the given input plane. It tries to stay visible 
*    for as long as possible, by moving the fixed number of segments with the camera's
*    position and adjusting the sub grid based on the camera's distance to the given 
*    plane. Additionally, the grid is rendered with a smooth circular mask centered 
*    at the perpendicular to the plane. Also near and far planes are considered, 
*    causing a smooth alpha transition near the far plane. 
*
*    Note: Any camera change causing view frustum changes, needs to be passed to 
*    the grid inn order to work correctly. The grid is drawn using transparency 
*    with OpenGL blending and depth buffer enabled.
*
*    Typeical usage of the AdaptiveGrid:
*    \code{.cpp}
*
*        AdaptiveGrid * agrid = new AdaptiveGrid(16U);
*        ...
*        agrid->setNearFar(0.1f, 1024.f);
*        agrid->update(eye, projection * view);
*        agrid->draw();
*
*    \endcode
*/
class GLOPERATE_API AdaptiveGrid : public globjects::Referenced
{
public:
    AdaptiveGrid(
        unsigned short segments = 16
    ,   const glm::vec3 & location = glm::vec3(0.f, 0.f, 0.f)
    ,   const glm::vec3 & normal = glm::vec3(0.f, 1.f, 0.f));

    void setColor(const glm::vec3 & color);

    /**
    *  @brief
    *    Update without arguments tries to access the set camera.
    *
    *    It accesses znear, zfar, eye, and viewProjection matrix.
    *    Note: If no camera is set, nothing happens.
    *
    *  @see setCamera
    *  @see setNearFar
    */
    void update();
    void setCamera(const Camera * camera);

    void update(
        const glm::vec3 & viewpoint
    ,   const glm::mat4 & modelViewProjection);

    void setNearFar(float zNear, float zFar);

    void draw();


private:
    void setupGridLineBuffer(unsigned short segments);


private:
    globjects::ref_ptr<globjects::Program> m_program;

    globjects::ref_ptr<globjects::VertexArray> m_vao;
    globjects::ref_ptr<globjects::Buffer> m_buffer;

    const Camera * m_camera;

    glm::vec3 m_location;
    glm::vec3 m_normal;

    glm::mat4 m_transform;

    unsigned short m_size;


protected:
    virtual ~AdaptiveGrid();
};


} // namespace globjectsutils
