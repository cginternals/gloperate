#include <gloperate-osg/OsgPainter.h>
#include <osg/Node>
#include <osgViewer/Viewer>
#include <gloperate/capabilities/ViewportCapability.h>
#include <gloperate/capabilities/InputCapability.h>
#include <gloperate-osg/OsgMouseHandler.h>
#include <gloperate-osg/OsgKeyboardHandler.h>


using namespace gloperate;
namespace gloperate_osg
{


void OsgPainter::osg_setScene(osg::Node * scene)
{
    // Release old scene
    if (m_scene) {
        m_scene->unref();
    }

    // Set new scene
    m_scene = scene;
    m_scene->ref();
    if (m_viewer) {
        m_viewer->setSceneData(m_scene);
    }
}

void OsgPainter::osg_onInitialize()
{
    // Release old data
    if (m_viewer) {
        m_viewer->unref();
    }
    if (m_embedded) {
        m_embedded->unref();
    }

    // Create OSG viewer
    m_viewer = new osgViewer::Viewer;
    m_viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
    m_viewer->ref();

    // Setup viewer using the already created window and OpenGL context
    m_embedded = m_viewer->setUpViewerAsEmbeddedInWindow(0, 0, 800, 600);
    m_embedded->ref();

    // Initialize camera
    m_viewer->getCamera()->setProjectionMatrixAsPerspective(30.0f, 1.0, 1.0f, 10000.0f);
    m_viewer->getCamera()->setViewMatrix(osg::Matrix::lookAt(osg::Vec3(0, 0, 50), osg::Vec3(0, 0, 0), osg::Vec3(0, 1, 0))); 

    // Initialize viewer
    if (m_scene) {
        m_viewer->setSceneData(m_scene);
    }
    m_viewer->realize();

    // Initialize input handlers
    m_inputCapability->addMouseHandler   (new OsgMouseHandler   (m_embedded));
    m_inputCapability->addKeyboardHandler(new OsgKeyboardHandler(m_embedded));
}

void OsgPainter::osg_onPaint()
{
    // Check if painter has been initialized correctly
    if (m_viewer && m_embedded) {
        // Send resize-event
        if (m_viewportX != m_viewportCapability->x() || m_viewportY != m_viewportCapability->y() ||
            m_viewportWidth != m_viewportCapability->width() || m_viewportHeight != m_viewportCapability->height() )
        {
            m_viewportX      = m_viewportCapability->x();
            m_viewportY      = m_viewportCapability->y();
            m_viewportWidth  = m_viewportCapability->width();
            m_viewportHeight = m_viewportCapability->height();

            m_embedded->resized(m_viewportX, m_viewportY, m_viewportWidth, m_viewportHeight);
            m_embedded->getEventQueue()->windowResize(m_viewportX, m_viewportY, m_viewportWidth, m_viewportHeight);
        }

        // Draw OSG scene
        m_viewer->frame();
    }
}

void OsgPainter::osg_cleanup()
{
    if (m_scene) {
        m_scene->unref();
    }

    if (m_embedded) {
        m_embedded->unref();
    }

    if (m_viewer) {
        m_viewer->unref();
    }
}


} // namespace gloperate_osg
