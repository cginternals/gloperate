#include <gloperate-osg/OsgRenderStage.h>

#include <osg/Node>
#include <osgViewer/Viewer>

#include <gloperate/capabilities/AbstractViewportCapability.h>
#include <gloperate-osg/OsgMouseHandler.h>
#include <gloperate-osg/OsgKeyboardHandler.h>


using namespace gloperate;
namespace gloperate_osg
{


void OsgRenderStage::osg_setScene(osg::Node * scene)
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

void OsgRenderStage::osg_initialize()
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
}

void OsgRenderStage::osg_process()
{
    // Check if painter has been initialized correctly
    if (m_viewer && m_embedded) {
        // Send resize-event
        if (m_viewport.data() && (
            m_viewportX != m_viewport.data()->x() || m_viewportY != m_viewport.data()->y() ||
            m_viewportW != m_viewport.data()->width() || m_viewportH != m_viewport.data()->height()) )
        {
            // Set new viewport
            m_viewportX = m_viewport.data()->x();
            m_viewportY = m_viewport.data()->y();
            m_viewportW = m_viewport.data()->width();
            m_viewportH = m_viewport.data()->height();
            m_embedded->resized(m_viewportX, m_viewportY, m_viewportW, m_viewportH);
            m_embedded->getEventQueue()->windowResize(m_viewportX, m_viewportY, m_viewportW, m_viewportH);

            // Inform sub-classes about changed viewport
            handleViewportChanged();
        }

        // Draw OSG scene
        m_viewer->frame();

        // Invoke post-render callback
        postOsgRendering();
    }
}

void OsgRenderStage::osg_cleanup()
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
