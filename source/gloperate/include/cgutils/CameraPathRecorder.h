#pragma once

#include <cgutils/cgutils_api.h>

#include <cgutils/CameraPath.h>

namespace cgutils
{
class Camera;

class CGUTILS_API CameraPathRecorder
{
public:
    CameraPathRecorder(Camera & camera);

    void record();
protected:
    Camera& m_camera;
    CameraPath m_path;
};

} // namespace gloutils
