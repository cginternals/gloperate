/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <gloperate-osg/OsgKeyboardHandler.h>
#include <osgViewer/Viewer>


using namespace gloperate;
namespace gloperate_osg
{


OsgKeyboardHandler::OsgKeyboardHandler(osgViewer::GraphicsWindowEmbedded * embedded)
: m_embedded(embedded)
{
}

OsgKeyboardHandler::~OsgKeyboardHandler()
{
}

void OsgKeyboardHandler::onKeyDown(gloperate::Key key)
{
    m_embedded->getEventQueue()->keyPress(key);
}

void OsgKeyboardHandler::onKeyUp(gloperate::Key key)
{
    m_embedded->getEventQueue()->keyRelease(key);
}


} // namespace gloperate_osg
