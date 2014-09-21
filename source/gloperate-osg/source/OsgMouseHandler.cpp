/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <gloperate-osg/OsgMouseHandler.h>
#include <osgViewer/Viewer>


using namespace gloperate;
namespace gloperate_osg
{


/**
*  @brief
*    Convert gloperate mouse button into OSG mouse button
*/
static int toOsgMouseButton(int button)
{
    if (button == 1)
        return 1;
    else if (button == 2)
        return 3;
    else if (button == 3)
        return 2;
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
    m_embedded->getEventQueue()->mouseMotion((float)x, (float)y);
}

void OsgMouseHandler::onMousePress(int x, int y, int button)
{
    m_embedded->getEventQueue()->mouseButtonPress((float)x, (float)y, toOsgMouseButton(button));
}

void OsgMouseHandler::onMouseRelease(int x, int y, int button)
{
    m_embedded->getEventQueue()->mouseButtonRelease((float)x, (float)y, toOsgMouseButton(button));
}

void OsgMouseHandler::onMouseDoubleClick(int x, int y, int button)
{
    m_embedded->getEventQueue()->mouseDoubleButtonPress((float)x, (float)y, toOsgMouseButton(button));
}


} // namespace gloperate_osg
