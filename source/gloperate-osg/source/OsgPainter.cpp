/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <gloperate-osg/OsgPainter.h>


using namespace gloperate;
namespace gloperate_osg
{


/**
*  @brief
*    Constructor
*/
OsgPainter::OsgPainter()
{
}

/**
*  @brief
*    Destructor
*/
OsgPainter::~OsgPainter()
{
}

/**
*  @brief
*    Get OSG scene
*/
osg::Node * OsgPainter::scene() const
{
	return m_scene;
}

/**
*  @brief
*    Set OSG scene
*/
void OsgPainter::setScene(osg::Node * scene)
{
	m_scene = scene;
    m_viewer->setSceneData(m_scene);
}

void OsgPainter::onInitialize()
{
	// Create OSG viewer
    m_viewer = new osgViewer::Viewer;
    m_embedded = m_viewer->setUpViewerAsEmbeddedInWindow(0, 0, 800, 600);

    // Initialize camera
    m_viewer->getCamera()->setProjectionMatrixAsPerspective(45.0, 1.0, 0.5, 1000);
    m_viewer->getCamera()->setViewMatrix(osg::Matrix::lookAt(osg::Vec3(0, 0, 50), osg::Vec3(0, 0, 0), osg::Vec3(0, 1, 0))); 

    // Initialize viewer
	m_viewer->realize();
}

void OsgPainter::onPaint()
{
    // [TODO] Implement using ViewportCapability
    /*
    // Send resize-event
    m_embedded->resized(viewport.x(), viewport.y(), viewport.width(), viewport.height());
    */

	// Draw OSG scene
	m_viewer->frame();
}


} // namespace gloperate_osg
