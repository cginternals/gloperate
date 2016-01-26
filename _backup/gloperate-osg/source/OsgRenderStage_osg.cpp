
#include <gloperate-osg/OsgRenderStage.h>

#include <glm/glm.hpp>

#include <osg/Node>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>

#include <gloperate-osg/OsgMouseHandler.h>
#include <gloperate-osg/OsgKeyboardHandler.h>


using namespace gloperate;


namespace gloperate_osg
{


void OsgRenderStage::osg_loadScene(const std::string & filename)
{
    setScene(osgDB::readNodeFile(filename));
}

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
    m_viewer->getCamera()->setProjectionMatrixAsPerspective(30.0f, 1.0, 0.01f, 10000.0f);
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
        if (m_viewportX != m_viewport.data().x || m_viewportY != m_viewport.data().y ||
            m_viewportW != m_viewport.data().z || m_viewportH != m_viewport.data().w )
        {
            // Set new viewport
            m_viewportX = m_viewport.data().x;
            m_viewportY = m_viewport.data().y;
            m_viewportW = m_viewport.data().z;
            m_viewportH = m_viewport.data().w;
            m_embedded->resized(m_viewportX, m_viewportY, m_viewportW, m_viewportH);
            m_embedded->getEventQueue()->windowResize(m_viewportX, m_viewportY, m_viewportW, m_viewportH);

            // Inform sub-classes about changed viewport
            handleViewportChanged();
        }

        // Draw OSG scene
        m_viewer->frame();

        // Update view and projection matrices
        m_projectionMatrix = convertMatrix( m_viewer->getCamera()->getProjectionMatrix() );
        m_viewMatrix       = convertMatrix( m_viewer->getCamera()->getViewMatrix() );

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

glm::mat4 OsgRenderStage::convertMatrix(const osg::Matrixd & mat) const
{
    const double * data = mat.ptr();
    return glm::mat4( (float)data[ 0], (float)data[ 1], (float)data[ 2], (float)data[ 3],
                      (float)data[ 4], (float)data[ 5], (float)data[ 6], (float)data[ 7],
                      (float)data[ 8], (float)data[ 9], (float)data[10], (float)data[11],
                      (float)data[12], (float)data[13], (float)data[14], (float)data[15] );
}


} // namespace gloperate_osg
