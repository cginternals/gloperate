#include <glbinding/gl/gl.h>

#include <globjects/logging.h>

#include <gloperate-osg/OsgSceneStage.h>
#include <gloperate/capabilities/ViewportCapability.h>
#include <gloperate/capabilities/TargetFramebufferCapability.h>
#include <gloperate/capabilities/InputCapability.h>
#include <gloperate/capabilities/VirtualTimeCapability.h>


using namespace gl;
using namespace gloperate;
namespace gloperate_osg
{


/**
*  @brief
*    Constructor
*/
OsgSceneStage::OsgSceneStage(const std::string & name)
: AbstractStage(name)
, m_viewer(nullptr)
, m_embedded(nullptr)
, m_scene(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
OsgSceneStage::~OsgSceneStage()
{
    osg_cleanup();
}

/**
*  @brief
*    Get OSG viewer
*/
osgViewer::Viewer * OsgSceneStage::viewer() const
{
    return m_viewer;
}

/**
*  @brief
*    Get OSG scene
*/
osg::Node * OsgSceneStage::scene() const
{
    return m_scene;
}

/**
*  @brief
*    Set OSG scene
*/
void OsgSceneStage::setScene(osg::Node * scene)
{
    osg_setScene(scene);
}

void OsgSceneStage::initialize()
{
    osg_initialize();
}

void OsgSceneStage::process()
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
