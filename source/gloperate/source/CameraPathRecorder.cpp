#include <cgutils/CameraPathRecorder.h>

#include <cgutils/Camera.h>

namespace cgutils
{

CameraPathRecorder::CameraPathRecorder(Camera & camera)
: m_camera(camera)
{
}

void CameraPathRecorder::record()
{
    m_path.append(CameraPathPoint(m_camera));
}

} // namespace gloutils
