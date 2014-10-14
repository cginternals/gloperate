#include <gloperate-osg/OsgMouseHandler.h>
#include <osgViewer/Viewer>


using namespace gloperate;
namespace gloperate_osg
{


static int toOsgMouseButton(gloperate::MouseButton button)
{
    if (button == MouseButtonLeft)
        return 1;
    else if (button == MouseButtonRight)
        return 3;
    else if (button == MouseButtonMiddle)
        return 2;
    else if (button >= MouseButton4)
        return static_cast<int>(button);
    else
        return 0;
}


OsgMouseHandler::OsgMouseHandler(osgViewer::GraphicsWindowEmbedded * embedded)
: m_embedded(embedded)
{
}

OsgMouseHandler::~OsgMouseHandler()
{
}

void OsgMouseHandler::onMouseMove(int x, int y)
{
    m_embedded->getEventQueue()->mouseMotion(static_cast<float>(x), static_cast<float>(y));
}

void OsgMouseHandler::onMousePress(int x, int y, gloperate::MouseButton button)
{
    m_embedded->getEventQueue()->mouseButtonPress(static_cast<float>(x), static_cast<float>(y), toOsgMouseButton(button));
}

void OsgMouseHandler::onMouseRelease(int x, int y, gloperate::MouseButton button)
{
    m_embedded->getEventQueue()->mouseButtonRelease(static_cast<float>(x), static_cast<float>(y), toOsgMouseButton(button));
}

void OsgMouseHandler::onMouseDoubleClick(int x, int y, gloperate::MouseButton button)
{
    m_embedded->getEventQueue()->mouseDoubleButtonPress(static_cast<float>(x), static_cast<float>(y), toOsgMouseButton(button));
}

void OsgMouseHandler::onMouseWheel(int dx, int dy)
{
    if (dx != 0) {
        m_embedded->getEventQueue()->mouseScroll(dx > 0 ? osgGA::GUIEventAdapter::SCROLL_LEFT : osgGA::GUIEventAdapter::SCROLL_RIGHT);
    }
    if (dy != 0) {
        m_embedded->getEventQueue()->mouseScroll(dy > 0 ? osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN);
    }
}


} // namespace gloperate_osg
