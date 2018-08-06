
#pragma once


#include <gloperate/rendering/PerspectiveCamera.h>


namespace gloperate
{

/**
*  @brief
*    Represents matrices for a typical 3D perspective look-at camera.
*
*    A camera is specified via near, far, fovy, as well as an eye, a center, and an up
*    vector. Camera itself does not use any OpenGL calls, but merely provides lazy
*    math to all common matrices required for affine transformation of a scene,
*    namely the view and projection matrices, their combination and all related
*    inverses (as well as a normal matrix).
*    The class relies on lazy computation of all matrices, causing less recomputations
*    of, e.g., matrices and inverse matrices requested on an irregular basis.
*/
class Camera : public PerspectiveCamera
{
public:
    using PerspectiveCamera::PerspectiveCamera;
};


} // namespace gloperate
