
#include <gloperate-osg/OsgMouseHandler.h>

#include <osgViewer/Viewer>
#include <osgViewer/GraphicsWindow>

#include <gloperate-osg/OsgRenderStage.h>


using namespace gloperate;

namespace gloperate_osg
{


/**
*  @brief
*    Convert gloperate mouse button into OSG mouse button
*/
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


OsgMouseHandler::OsgMouseHandler(osgViewer::GraphicsWindowEmbedded * embedded, OsgRenderStage * stage)
: m_embedded(embedded)
, m_stage(stage)
{
	if (m_embedded) {
		m_embedded->ref();
	}
}

OsgMouseHandler::~OsgMouseHandler()
{
	if (m_embedded) {
		m_embedded->unref();
	}
}

void OsgMouseHandler::onMouseMove(int x, int y)
{
	if (m_embedded) m_embedded->getEventQueue()->mouseMotion(static_cast<float>(x), static_cast<float>(y));

    if (m_stage) m_stage->scheduleProcess();
}

void OsgMouseHandler::onMousePress(int x, int y, gloperate::MouseButton button)
{
	if (m_embedded) m_embedded->getEventQueue()->mouseButtonPress(static_cast<float>(x), static_cast<float>(y), toOsgMouseButton(button));

    if (m_stage) m_stage->scheduleProcess();
}

void OsgMouseHandler::onMouseRelease(int x, int y, gloperate::MouseButton button)
{
	if (m_embedded) m_embedded->getEventQueue()->mouseButtonRelease(static_cast<float>(x), static_cast<float>(y), toOsgMouseButton(button));

    if (m_stage) m_stage->scheduleProcess();
}

void OsgMouseHandler::onMouseDoubleClick(int x, int y, gloperate::MouseButton button)
{
	if (m_embedded) m_embedded->getEventQueue()->mouseDoubleButtonPress(static_cast<float>(x), static_cast<float>(y), toOsgMouseButton(button));

    if (m_stage) m_stage->scheduleProcess();
}

void OsgMouseHandler::onMouseWheel(int dx, int dy)
{
    if (dx != 0) {
		if (m_embedded) m_embedded->getEventQueue()->mouseScroll(dx > 0 ? osgGA::GUIEventAdapter::SCROLL_LEFT : osgGA::GUIEventAdapter::SCROLL_RIGHT);
    }
    if (dy != 0) {
		if (m_embedded) m_embedded->getEventQueue()->mouseScroll(dy > 0 ? osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN);
    }

    if (m_stage) m_stage->scheduleProcess();
}


} // namespace gloperate_osg
