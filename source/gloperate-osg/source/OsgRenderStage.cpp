#include <gloperate-osg/OsgRenderStage.h>

#include <glbinding/gl/gl.h>

#include <globjects/logging.h>

#include <gloperate/capabilities/ViewportCapability.h>
#include <gloperate/capabilities/TargetFramebufferCapability.h>
#include <gloperate/capabilities/InputCapability.h>
#include <gloperate/capabilities/VirtualTimeCapability.h>

#include <gloperate-osg/OsgKeyboardHandler.h>
#include <gloperate-osg/OsgMouseHandler.h>


using namespace gl;
using namespace gloperate;
namespace gloperate_osg
{


/**
*  @brief
*    Constructor
*/
OsgRenderStage::OsgRenderStage(const std::string & name)
: AbstractStage(name)
, m_viewer(nullptr)
, m_embedded(nullptr)
, m_scene(nullptr)
, m_viewportX(0)
, m_viewportY(0)
, m_viewportWidth(800)
, m_viewportHeight(600)
{
}

/**
*  @brief
*    Destructor
*/
OsgRenderStage::~OsgRenderStage()
{
    osg_cleanup();
}

/**
*  @brief
*    Get OSG viewer
*/
osgViewer::Viewer * OsgRenderStage::viewer() const
{
    return m_viewer;
}

/**
*  @brief
*    Get OSG scene
*/
osg::Node * OsgRenderStage::scene() const
{
    return m_scene;
}

/**
*  @brief
*    Set OSG scene
*/
void OsgRenderStage::setScene(osg::Node * scene)
{
    osg_setScene(scene);
}

/**
*  @brief
*    Set viewport
*/
void OsgRenderStage::setViewport(int x, int y, int width, int height)
{
    m_viewportX      = x;
    m_viewportY      = y;
    m_viewportWidth  = width;
    m_viewportHeight = height;
}

/**
*  @brief
*    Create keyboard handler to control the wrapped OSG scene
*/
OsgKeyboardHandler * OsgRenderStage::createKeyboardHandler() const
{
    if (m_embedded) {
        return new OsgKeyboardHandler(m_embedded);
    } else {
        return nullptr;
    }
}

/**
*  @brief
*    Create mouse handler to control the wrapped OSG scene
*/
OsgMouseHandler * OsgRenderStage::createMouseHandler() const
{
    if (m_embedded) {
        return new OsgMouseHandler(m_embedded);
    } else {
        return nullptr;
    }
}

void OsgRenderStage::initialize()
{
    osg_initialize();
}

void OsgRenderStage::process()
{
    // Get framebuffer to render into
    // [TODO] How to pass this to the stage?
    /*
    globjects::Framebuffer * fbo = m_targetFramebufferCapability->framebuffer();
    if (!fbo) {
        fbo = globjects::Framebuffer::defaultFBO();
    }

    // Bind framebuffer
    fbo->bind(GL_FRAMEBUFFER);
    */

    // Draw osg scene
    osg_process();
}


} // namespace gloperate_osg
